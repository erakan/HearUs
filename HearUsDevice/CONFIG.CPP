/**
 * HearUs ~ Announcement device application and client for DM&P's Mity-Mite Module
 * Copyright (c) 2005 Erdinc AKAN (erakan@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 **/

#include <stdio.h>
#include <ctype.h>
int main(void)
{
	char ip[255], nm[255],gw[255],ns[255], c;
	printf("HEARUS Configurer\n");
	printf("To work correctly you must enter all the information!\n");
renter:
	printf("This terminals;\nIP Number:");
	scanf("%s",ip);
	printf("Net Mask:");
	scanf("%s", nm);
	printf("Gateway:");
	scanf("%s", gw);
	printf("Name Server:");
	scanf("%s", ns);
	printf("Is this information correct:\nIP:%s\nNetMask:%s\nGateWay:%s\nNameServer:%s\nSave Config File (Y)es to save, (N)o to re enter, (Q) to quit: ",
	 ip, nm, gw, ns);
    getchar();
	c = getchar();
    c=toupper(c);
	if(c=='N') goto renter;
	if(c=='Y')
	{
		printf("Saving config file...");
		FILE *f= fopen("hearus.cfg", "w");
		if(f)
		{
			printf(" Succeded!\n");
			fprintf(f, "ip=%s\nnetmask=%s\ngateway=%s\nnameserver=%s\n",
            ip,nm,gw,ns);
			fclose(f);
		}
		else
			printf(" Failed!\n");
	}

	return 0;
}