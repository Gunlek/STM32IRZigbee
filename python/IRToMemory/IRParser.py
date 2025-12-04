from typing import Dict

from IRCommand import *


class IRParser:
    def __init__(self, path):
        self.ir_commands: Dict[str, IRCommand] = {}

        self.file = open(path, 'r')
        self.parse()

    def parse(self):
        """
            Parse the file stored in self.file and population self.ir_commands accordingly
        """
        sections = [""]
        current_section_index = -1

        """
            SECTION STRUCTURE:
                name: <string>
                type: <string>
                frequency: <number>
                duty_cycle: <float>
                data: <space separated list of number>
        """

        # --- Read the IR file, line by line
        for line in self.file.readlines():
            # --- Each section is delimited by one line containing only "#"
            if line.startswith('#'):
                sections[current_section_index] = sections[current_section_index].rstrip('\n ')
                current_section_index += 1
                continue

            # --- The first section is ignored as it contains general info about the acquisition
            if current_section_index >= 0:
                # --- Create an empty section if it is the first line of the section to be processed
                if len(sections) < current_section_index + 1:
                    sections += [""]
                # --- Add the line to the section
                sections[current_section_index] += line

        # --- Remove the line return from the last section
        sections[current_section_index] = sections[current_section_index].rstrip('\n ')

        # --- Sections will be stored in a directory where the keys are the section's names
        for section in sections:
            section_split = section.split('\n')

            # --- Section description will contain each section info: name, type, frequency, duty_cycle, data
            section_description = {}
            for raw_line in section_split:
                [key, value] = raw_line.split(': ')
                section_description[key] = value

            self.ir_commands[section_description['name']] = IRCommand.from_dict(section_description)

    def get_available_commands(self):
        """
            Returns a list of available commands as per the parsed file
            :returns: The list of available commands, as a list of string
        """
        return list(self.ir_commands.keys())

    def get_command(self, name):
        """
        Returns a command as per the parsed file
            :param name: The name of the command
            :return: Command's structure and details
        """
        return self.ir_commands[name]

"""
    NOT HANDLED FOR NOW:
    
    requested_cmd = input('Command name to be traced: ')

    if requested_cmd not in section_dict.keys():
        print('Error: This command is not in the provided file')
        exit()

    return section_dict, requested_cmd
"""