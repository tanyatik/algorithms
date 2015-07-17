#!/usr/bin/env python

import sys
import re


def substitute_includes(source_file_name, new_file_name, include_path):
    print source_file_name, new_file_name, include_path
    include_regex = re.compile("[#]include \"([a-zA-Z\/._-]+)\"")

    with open(source_file_name, 'r') as source_file:
        with open(new_file_name, 'w') as new_file:
            for source_line in source_file.readlines():
                match = include_regex.match(source_line)

                if (match is not None):
                    include_filename = match.group(1)
                    with open(include_path + include_filename, 'r') as include_file:
                        include_content = include_file.read()
                        new_file.write(include_content)
                    try:
                        source_filename = include_filename.replace(".hpp", ".cpp")
                        with open(include_path + source_filename, 'r') as source_file:
                            include_content = source_file.read()
                            new_file.write(include_content)
                    except IOError:
                        print "No source file, pass"
                        pass
                else:
                    new_file.write(source_line)


if __name__ == "__main__":
    if len(sys.argv) > 1:
        source_file = sys.argv[1]

    include_path = "/Users/tsborisova/code/"

    if len(sys.argv) > 2:
        include_path = sys.argv[2]

    substitute_includes(source_file + ".cpp", source_file + "_send.cpp", include_path)
