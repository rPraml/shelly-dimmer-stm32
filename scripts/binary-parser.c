/* 
 * This file is part of the shelly-dimmer-stm32 project.
 * https://github.com/jamesturton/shelly-dimmer-stm32
 * Copyright (c) 2020 James Turton.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	FILE *fileptr;
	unsigned char *buffer;
	long filelen;
	int i = 0;

    if (argc != 5)
	{
        fprintf(stderr, "USAGE: %s INPUT_FILE OUTPUT_FILE MAJOR_VERSION MINOR_VERSION\n", argv[0]);
        exit(1);
    }

	fileptr = fopen(argv[1], "rb");     
	if (fileptr == NULL)
	{
		fprintf(stderr, "Cannot open input file: %s\n", argv[1]);
		return 1;
    }

	fseek(fileptr, 0, SEEK_END);          
	filelen = ftell(fileptr);            
	rewind(fileptr);                      
	buffer = (unsigned char *)malloc((filelen+1)*sizeof(unsigned char)); 

	fread(buffer, filelen, 1, fileptr);

	fclose(fileptr);

	fileptr = fopen(argv[2], "w");
	if (fileptr == NULL)
	{
		fprintf(stderr, "Cannot open ouput file: %s\n", argv[2]);
		return 1;
    }
	
	fprintf(fileptr, "static const uint8_t SHD_FIRMWARE_MAJOR_VERSION = %d;\n", atoi(argv[3]));
	fprintf(fileptr, "static const uint8_t SHD_FIRMWARE_MINOR_VERSION = %d;\n", atoi(argv[4]));
	fprintf(fileptr, "const uint8_t stm_firmware[] PROGMEM = {\n");
	while (i < filelen)
	{
		fprintf(fileptr, "    ");
		for (int j = 0; j < 12; j++)
			fprintf(fileptr, "0x%.2x, ", buffer[i++]);
		fprintf(fileptr, "\n");
	}
	fprintf(fileptr, "};\n\n");

	return 0;
}
