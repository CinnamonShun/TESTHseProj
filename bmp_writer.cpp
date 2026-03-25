#include "bmp_writer.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <limits>

#include "bmp_format.h"
#include "exceptions.h"

constexpr uint16_t KBmpSignature = 0x4D42;
constexpr uint16_t KBitsPerPixel = 24;
constexpr uint32_t KCompression = 0;

size_t CalculatePaddingWriter(const size_t width) {
    const size_t row_bytes = width * 3;
    return (4 - (row_bytes % 4)) % 4;
}

void ValidateDimensions(const size_t width, const size_t height) {
    if (width == 0 || height == 0) {
        throw InvalidArgumentsException("image has zero width or height");
    }

    if (width > static_cast<size_t>(std::numeric_limits<int32_t>::max())) {
        throw InvalidArgumentsException("image width is too large for BMP");
    }

    if (height > static_cast<size_t>(std::numeric_limits<int32_t>::max())) {
        throw InvalidArgumentsException("image height is too large for BMP");
    }
}

BmpFileHeader BuildFileHeader(const size_t data_size) {
    const size_t headers_size = sizeof(BmpFileHeader) + sizeof(BmpInfoHeader);
    const size_t file_size = headers_size + data_size;

    if (file_size > std::numeric_limits<uint32_t>::max()) {
        throw InvalidArgumentsException("image is too large for BMP file size field");
    }

    BmpFileHeader file_header{};
    file_header.signature = KBmpSignature;
    file_header.file_size = static_cast<uint32_t>(file_size);
    file_header.reserved1 = 0;
    file_header.reserved2 = 0;
    file_header.pixel_data_offset = static_cast<uint32_t>(headers_size);

    return file_header;
}

BmpInfoHeader BuildInfoHeader(const size_t width, const size_t height, const size_t data_size) {
    if (data_size > std::numeric_limits<uint32_t>::max()) {
        throw InvalidArgumentsException("image is too large for BMP data size field");
    }

    BmpInfoHeader info_header{};
    info_header.header_size = sizeof(BmpInfoHeader);
    info_header.width = static_cast<int32_t>(width);
    info_header.height = static_cast<int32_t>(height);
    info_header.planes = 1;
    info_header.bits_per_pixel = KBitsPerPixel;
    info_header.compression = KCompression;
    info_header.image_size = static_cast<uint32_t>(data_size);
    info_header.x_pixels_per_meter = 0;
    info_header.y_pixels_per_meter = 0;
    info_header.colors_used = 0;
    info_header.colors_important = 0;

    return info_header;
}

size_t CalculateDataSize(const size_t width, const size_t height) {
    if (width > std::numeric_limits<std::size_t>::max() / 3) {
        throw InvalidArgumentsException("image is too large");
    }

    const size_t row_bytes = width * 3;
    const size_t padding = CalculatePaddingWriter(width);

    if (row_bytes > std::numeric_limits<size_t>::max() - padding) {
        throw InvalidArgumentsException("image is too large");
    }

    const size_t stride = row_bytes + padding;
    if (stride != 0 && height > std::numeric_limits<size_t>::max() / stride) {
        throw InvalidArgumentsException("image is too large");
    }

    return stride * height;
}

void BmpWriter::Write(const std::string& path, const Image& image) const {
    const size_t width = image.GetWidth();
    const size_t height = image.GetHeight();

    ValidateDimensions(width, height);

    const size_t padding = CalculatePaddingWriter(width);
    const size_t data_size = CalculateDataSize(width, height);

    const BmpFileHeader file_header = BuildFileHeader(data_size);
    const BmpInfoHeader info_header = BuildInfoHeader(width, height, data_size);

    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw FileWriteException(path);
    }

    if (!file.write(reinterpret_cast<const char*>(&file_header), sizeof(file_header))) {
        throw FileWriteException(path);
    }

    if (!file.write(reinterpret_cast<const char*>(&info_header), sizeof(info_header))) {
        throw FileWriteException(path);
    }

    std::array<char, 3> padding_bytes{};

    for (size_t output_row = 0; output_row < height; ++output_row) {
        const size_t image_row = height - 1 - output_row;

        for (size_t x = 0; x < width; ++x) {
            const Color& pixel = image.At(x, image_row);
            const std::array<uint8_t, 3> bgr{pixel.GetBlue(), pixel.GetGreen(), pixel.GetRed()};

            if (!file.write(reinterpret_cast<const char*>(bgr.data()), static_cast<std::streamsize>(bgr.size()))) {
                throw FileWriteException(path);
            }
        }

        if (padding > 0) {
            if (!file.write(padding_bytes.data(), static_cast<std::streamsize>(padding))) {
                throw FileWriteException(path);
            }
        }
    }
}

void BMPWriter(const std::string& path, const Image& image) {
    const BmpWriter writer;
    writer.Write(path, image);
}
