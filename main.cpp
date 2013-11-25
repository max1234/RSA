#include "rsa.h"
int main(int argc, char * argv[])
{
    if(argc <  2)
    {
        printf("Some errors.Write -help for help\n");
        return 0;
    }
    else if(strcmp(argv[1],"-help") == 0)
    {
        printf("RSA.\nUse: rsa <opion> [FILES]\n");
        printf("\t -g|--generate [PublicKeyFILE] [PrivateKeyFILE] [RandomizeFILE] KeySizeInBits \n\t Generate RSA-keys\n");
        printf("\t -e|--encrypt [InputFILE] [OutputFILE] [PublicKeyFILE] \n\t Encrypt\n");
        printf("\t -d|--decrypt [InputFILE] [OutputFILE] [PrivateKeyFILE] \n\t Decrypt\n");
        printf("\t -h|--help \n\t Print this message\n");
        return 0;
    }
    else
    {
        if(argc >= 5)
        {
            RSA rsa;
            FILE *file1, *file2, *file3;

            if((strcmp(argv[1],"-g") == 0) || (strcmp(argv[1],"--generate") == 0))
            {
                unsigned int numBits;
                if(argc == 6)
                    numBits = atoi(argv[5]);
                else
                    numBits = 1024;

                if(numBits % 8)
                {
                    printf("Key size must be a multiple of 8\n");
                    return 0;
                }
                else
                    printf("Key size = %u bits\n", numBits);

                file1 = fopen(argv[2],"w");
                file2 = fopen(argv[3],"w");
                file3 = fopen(argv[4],"r");

                if(!file1 || !file2 || !file3)
                    printf("Could not read one of files\n");
                else
                    rsa.GenerateKeys(file1, file2, file3, numBits);

                fclose(file1);
                fclose(file2);
                fclose(file3);
            }
            else if((strcmp(argv[1],"-e") == 0) || (strcmp(argv[1],"--encrypt") == 0))
            {
                file1 = fopen(argv[2],"rb");
                file2 = fopen(argv[3],"wb");
                file3 = fopen(argv[4],"r");

                if(!file1 || !file2 || !file3)
                    printf("Could not read one of files\n");
                else
                    rsa.Encrypt(file1, file2, file3);

                fclose(file1);
                fclose(file2);
                fclose(file3);
            }
            else if((strcmp(argv[1],"-d") == 0) || (strcmp(argv[1],"--decrypt") == 0))
            {
                file1 = fopen(argv[2],"rb");
                file2 = fopen(argv[3],"wb");
                file3 = fopen(argv[4],"r");

                if(!file1 || !file2 || !file3)
                    printf("Could not read one of files\n");
                else
                    rsa.Decrypt(file1, file2, file3);

                fclose(file1);
                fclose(file2);
                fclose(file3);
            }
            else
            {
                printf("Some errors.Write -help for help\n");
                return 0;
            }
        }
        else
        {
            printf("Some errors.Write -help for help\n");
            return 0;
        }
    }
    return 0;
}
