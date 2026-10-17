/***********************************************************************************************************************
**
** Copyright (C) 2021 BaseALT Ltd.
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**
***********************************************************************************************************************/

#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

#include <fstream>
#include <exception>
#include <iostream>

#include "policydefinitionfiles.h"

enum ReturnCodes
{
    STATUS_OK = 0,
    STATUS_OPTIONS_ERROR = 1,
    STATUS_VERIFICATION_ERROR = 2,
    STATUS_FORMAT_NOT_SUPPORTED = 3
};

int main(int argc, char ** argv)
{
    std::string input_file_path;

    try {
        using namespace boost::program_options;
        options_description options("input (required)");
        options.add_options()
                ("input_file", value<std::string>(&input_file_path)->required(), "file in ADMX/ADML format.")
                ("help", "help information")
                ("version", "version information");

        variables_map vm;
        store(parse_command_line(argc, argv, options), vm);        

        if (argc == 1 || vm.count("help"))
        {
            std::cout << options << std::endl;
            return STATUS_OK;
        }

        if (vm.count("version"))
        {
            std::cout << "0.1.0" << std::endl;
            return STATUS_OK;
        }

        notify(vm);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return STATUS_OPTIONS_ERROR;
    }

    bool status_ok = false;

    try
    {
        std::ifstream file;

        file.open(input_file_path, std::ifstream::in);

        if (file.good()) {
            const std::string lower = boost::algorithm::to_lower_copy(input_file_path);
            if (boost::algorithm::ends_with(lower, "admx"))
            {
                GroupPolicy::PolicyDefinitions::policyDefinitions(file, ::xsd::cxx::tree::flags::dont_validate);
            }
            else if (boost::algorithm::ends_with(lower, "adml"))
            {
                GroupPolicy::PolicyDefinitions::policyDefinitionResources(file, ::xsd::cxx::tree::flags::dont_validate);
            }
            else
            {
                std::cout << "Input format of file: " << input_file_path << " is not supported." << std::endl;
                return STATUS_FORMAT_NOT_SUPPORTED;
            }

            status_ok = true;
        } else {
            std::cout << "Unable to open file: " << input_file_path << std::endl;
        }

        file.close();
    }
    catch (const xsd::cxx::tree::unexpected_element<char>& e)
    {
        std::cout << "Error an unexpected element: " << e.encountered_name() << " in namespace: "
                  << e.encountered_namespace() << " was encountered. Expected element: "
                  << e.expected_name() << " in namespace: " << e.expected_namespace() << std::endl;
    }
    catch (const xsd::cxx::tree::expected_element<char>& e)
    {
        std::cout << "Error an expected element: " << e.name()
                  << " in namespace: " << e.namespace_() <<" was not encountered: " << std::endl;
    }
    catch (const xsd::cxx::tree::expected_attribute<char>& e)
    {
        std::cout << "Error an expected attribute: " << e.name()
                  << " in namespace: " << e.namespace_() <<" was not encountered: " << std::endl;
    }
    catch (const xsd::cxx::tree::unexpected_enumerator<char>& e)
    {
        std::cout << "Error an unexpected enumerator: " << e.enumerator() << std::endl;
    }
    catch (const xsd::cxx::tree::expected_text_content<char>& e)
    {
        std::cout << "Error text content was expected for an element." << e.what() << std::endl;
    }
    catch (const xsd::cxx::tree::no_element_info<char>& e)
    {
        std::cout << "Error no information available for element: " << e.element_name() << std::endl;
    }
    catch (const xsd::cxx::tree::no_type_info<char>& e)
    {
        std::cout << "Error no type information available for: " << e.type_name() << " in namespace: "
                  << e.type_namespace() << std::endl;
    }
    catch (const xsd::cxx::tree::duplicate_id<char>& e)
    {
        std::cout << "Error duplicate ID value was encountered in the object model: " << e.id() << std::endl;
    }
    catch (const xsd::cxx::tree::no_prefix_mapping<char>& e)
    {
        std::cout << "Error no prefix-namespace mapping was provided for prefix: " << e.prefix() << std::endl;
    }
    catch (const xsd::cxx::tree::bounds<char>& e)
    {
        std::cout << "Size of argument exceeds the capacity argument: " << e.what() << std::endl;
    }
    catch (const xsd::cxx::tree::exception<char>& e)
    {
        std::cout << "Verification error: " << e.what() << std::endl;
    }
    catch (const xsd::cxx::xml::invalid_utf16_string&)
    {
        std::cout << "Invalid UTF-16 text in DOM model." << std::endl;
    }
    catch (const xsd::cxx::xml::invalid_utf8_string&)
    {
        std::cout << "Invalid UTF-8 text in object model." << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    if (status_ok)
    {
        return STATUS_OK;
    }
    else
    {
        std::cout << "In file: " << input_file_path << std::endl;
        return STATUS_VERIFICATION_ERROR;
    }
}
