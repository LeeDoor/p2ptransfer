#include <catch2/catch_test_macros.hpp>
#include "../server/argument_parser.hpp"

SCENARIO ("ArgumentParser parses the data") {
    ArgumentData res;
    ArgumentParser parser;
    GIVEN ("an argument list as char**") {
        char** argv;
        WHEN ("trying to parse empty pointer") {
            bool success = parser.parse_arguments(0, argv, res);
            THEN ("should return false") {
                REQUIRE(success == false);
            }
        }
        WHEN ("Allocating memory for one argument and parsing") {
            argv = new char*;
            argv[0] = new char[]{"executable"};
            bool success = parser.parse_arguments(1, argv, res);
            THEN ("For any data parsing should fail") {
                REQUIRE(success == false);
            }
            delete[] argv[0];
            delete[] argv;
        }
        WHEN ("Allocating memory to 2 elements") {
            argv = new char*[2];
            argv[0] = new char[]{"executable"};
            argv[1] = new char[20];
            
            WHEN ("PORT is a positive number") {
                strcpy(argv[1], "1984");
                REQUIRE(parser.parse_arguments(2, argv, res));
                REQUIRE(res.port == 1984);

                strcpy(argv[1], "65535");
                REQUIRE(parser.parse_arguments(2, argv, res));
                REQUIRE(res.port == 65535);

                strcpy(argv[1], "0");
                REQUIRE(parser.parse_arguments(2, argv, res));
                REQUIRE(res.port == 0);

                strcpy(argv[1], "-0");
                REQUIRE(parser.parse_arguments(2, argv, res));
                REQUIRE(res.port == 0);

                strcpy(argv[1], "1489");
                REQUIRE(parser.parse_arguments(2, argv, res));
                REQUIRE(res.port == 1489);
            }
            WHEN ("PORT is negative value or a string") {
                strcpy(argv[1], "-1984");
                REQUIRE(!parser.parse_arguments(2, argv, res));

                strcpy(argv[1], "-65535");
                REQUIRE(!parser.parse_arguments(2, argv, res));

                strcpy(argv[1], "abobus");
                REQUIRE(!parser.parse_arguments(2, argv, res));

                strcpy(argv[1], "one");
                REQUIRE(!parser.parse_arguments(2, argv, res));

                strcpy(argv[1], "");
                REQUIRE(!parser.parse_arguments(2, argv, res));
            }

            delete[] argv[0];
            delete[] argv[1];
            delete[] argv;
        }
    }
}
