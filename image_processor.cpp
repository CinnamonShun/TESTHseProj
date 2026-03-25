#include <iostream>
#include <memory>
#include <vector>

#include "bmp_reader.h"
#include "bmp_writer.h"
#include "cli/arguments_parser.h"
#include "cli/filter_factory.h"
#include "cli/filter_pipeline.h"
#include "exceptions.h"
#include "project_sources.cpp"

int main(const int argc, char* argv[]) {
    const ArgumentsParser parser;

    try {
        const ProgramOptions options = parser.Parse(argc, argv);
        if (options.show_help) {
            std::cout << parser.GetHelp();
            return 0;
        }

        const FilterFactory factory;
        std::vector<std::unique_ptr<Filter>> filters = factory.Create(options.filter_requests);

        const BmpReader reader;
        const BmpWriter writer;

        const Image image = reader.Read(options.input_path);

        const FilterPipeline pipeline(std::move(filters));
        const Image result = pipeline.Apply(image);

        writer.Write(options.output_path, result);

        return 0;
    } catch (const InvalidArgumentsException& exception) {
        std::cerr << exception.what() << '\n';
        std::cerr << parser.GetHelp();
    } catch (const ImageProcessorException& exception) {
        std::cerr << exception.what() << '\n';
    } catch (const std::exception& exception) {
        std::cerr << "Unexpected error: " << exception.what() << '\n';
    }

    return 1;
}
