#include "bmp_reader.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <limits>

#include "bmp_format.h"
#include "exceptions.h"

namespace {

constexpr uint16_t kBmpSignature = 0x4D42;
constexpr uint16_t kSupportedBitsPerPixel = 24;
constexpr uint32_t kSupportedCompression = 0;

void ValidateHeaders(const BmpFileHeader& file_header, const BmpInfoHeader& info_header) {
    if (file_header.signature != kBmpSignature) {
        throw InvalidFormatException("file signature is not BM");
    }

    if (info_header.header_size != sizeof(BmpInfoHeader)) {
        throw InvalidFormatException("only BITMAPINFOHEADER is supported");
    }

    if (info_header.planes != 1) {
        throw InvalidFormatException("invalid planes count");
    }

    if (info_header.bits_per_pixel != kSupportedBitsPerPixel) {
        throw InvalidFormatException("only 24-bit BMP is supported");
    }

    if (info_header.compression != kSupportedCompression) {
        throw InvalidFormatException("compressed BMP is not supported");
    }

    if (info_header.width <= 0 || info_header.height == 0) {
        throw InvalidFormatException("invalid image dimensions");
    }

    if (info_header.colors_used != 0) {
        throw InvalidFormatException("color table is not supported");
    }

    const uint32_t min_pixel_offset = static_cast<uint32_t>(sizeof(BmpFileHeader) + sizeof(BmpInfoHeader));
    if (file_header.pixel_data_offset < min_pixel_offset) {
        throw InvalidFormatException("invalid pixel data offset");
    }
}

size_t GetHeightAbs(const int32_t height) {
    if (height == std::numeric_limits<int32_t>::min()) {
        throw InvalidFormatException("invalid image height");
    }

    const int64_t signed_height = static_cast<int64_t>(height);
    const int64_t positive_height = signed_height > 0 ? signed_height : -signed_height;

    return static_cast<size_t>(positive_height);
}

size_t CalculatePadding(const size_t width) {
    const size_t row_bytes = width * 3;
    return (4 - (row_bytes % 4)) % 4;
}

void ValidateImageSize(const size_t width, const size_t height) {
    if (width > std::numeric_limits<size_t>::max() / 3) {
        throw InvalidFormatException("image is too large");
    }

    const size_t row_bytes = width * 3;
    const size_t padding = CalculatePadding(width);

    if (row_bytes > std::numeric_limits<size_t>::max() - padding) {
        throw InvalidFormatException("image is too large");
    }

    const size_t stride = row_bytes + padding;
    if (stride != 0 && height > std::numeric_limits<size_t>::max() / stride) {
        throw InvalidFormatException("image is too large");
    }
}

void EnsureStreamSize(std::ifstream& file, const BmpFileHeader& file_header, const size_t data_size) {
    file.seekg(0, std::ios::end);
    const std::streamoff real_size = file.tellg();
    if (real_size < 0) {
        throw InvalidFormatException("cannot determine file size");
    }

    const uint64_t offset = file_header.pixel_data_offset;
    const uint64_t required_size = offset + data_size;
    if (required_size > static_cast<uint64_t>(real_size)) {
        throw InvalidFormatException("file is truncated");
    }

    file.seekg(static_cast<std::streamoff>(offset), std::ios::beg);
}

}  // namespace

Image BmpReader::Read(const std::string& path) const {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw FileReadException(path);
    }

    BmpFileHeader file_header{};
    BmpInfoHeader info_header{};

    if (!file.read(reinterpret_cast<char*>(&file_header), sizeof(file_header))) {
        throw InvalidFormatException("cannot read BMP file header");
    }

    if (!file.read(reinterpret_cast<char*>(&info_header), sizeof(info_header))) {
        throw InvalidFormatException("cannot read BMP info header");
    }

    ValidateHeaders(file_header, info_header);

    const size_t width = static_cast<size_t>(info_header.width);
    const size_t height = GetHeightAbs(info_header.height);

    ValidateImageSize(width, height);

    const size_t row_bytes = width * 3;
    const size_t padding = CalculatePadding(width);
    const size_t stride = row_bytes + padding;
    const size_t data_size = stride * height;

    EnsureStreamSize(file, file_header, data_size);

    const bool is_bottom_up = info_header.height > 0;

    Image image(width, height);

    std::array<char, 3> padding_bytes{};
    std::array<uint8_t, 3> bgr{};

    for (size_t file_row = 0; file_row < height; ++file_row) {
        const size_t image_row = is_bottom_up ? (height - 1 - file_row) : file_row;

        for (size_t x = 0; x < width; ++x) {
            if (!file.read(reinterpret_cast<char*>(bgr.data()), bgr.size())) {
                throw InvalidFormatException("unexpected end of file in pixel data");
            }
            image.At(x, image_row) = Color(bgr[2], bgr[1], bgr[0]);
        }

        if (padding > 0) {
            if (!file.read(padding_bytes.data(), static_cast<std::streamsize>(padding))) {
                throw InvalidFormatException("unexpected end of file in row padding");
            }
        }
    }

    return image;
}

Image BMPReader(const std::string& path) {
    const BmpReader reader;
    return reader.Read(path);
}
