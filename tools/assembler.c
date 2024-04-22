//SSOC assembler
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define VERSION "1.0.0"


typedef struct Definition
{
    unsigned char type; // 1: string, 2: int,  3: function, 4: subassembly. Only the first 2 are from DEFINE
    char * name;
    uint16_t value;
}Definition;


#define true 1
#define false 0

int defineInMemory(){
    return 0;
}

int memSize = 4096;

int main(int argc, char *argv[]){

    Definition defs[memSize];
    int defIndex = 0;

    printf("---SSOC ASSEMBLER V%s--- \n",VERSION);
    
    printf("Compiling SSOC Assembly at: %s \n",argv[1]);

        

    FILE * file = NULL;

          

    
    file = fopen(argv[1], "r");



    if(!file){
        printf("File not found, or something went very wrong! Exiting... \n");
        return -1;
    }



    int size = 128;



    char buffer[size];



    while (fgets(buffer, size, file) != NULL) // Gets the next line. simply printing here will not dump the string. You must manually iterate through tokens. 
    {


        
        char * tokens[size];

       char * tokenized;


        int init = false;
        tokenized =  strtok(buffer," ");

        tokens[0] = tokenized;

        if(strstr(tokens[0],"//")) { continue; } // if the first token contains double slashes, ignore it.


        // while our token isn't null (there are still tokens to process)

        int getDef = false;
        int getValue = false;

        Definition tempDef;

        while(tokenized != NULL){

            // get new token, but make sure to not reject the first token.
            if(init == false){ init = true;}else{
            tokenized = strtok(NULL, " ");
            }

            if(tokenized == NULL){break;}

            printf(tokenized);
            
            // we get a new token, and name our new def as it.
            if(getDef == true){
                getDef = false;
                tempDef.name = tokenized;
                continue;
            }

                        // we get a new token, and name our new def as it.
            if(getValue == true){
                getValue = false;
                if(!strstr(tokenized,"\"")){ // if we have a string def... (this needs extra processing)
                
                }else{ // integer values
                    printf("creating integer def named: %s value: %s", tempDef.name, *tokenized);
                    tempDef.value = *tokenized;
                }

                defs[defIndex] = tempDef;
                defIndex++;
                continue;
            }

       
     

            // when we find a Define...
            
            if(tokenized != NULL){
                
            if(strcmp(tokenized, "DEFINE") == 0){

                printf("creating def!");

                getDef = true; // prepare for naming
                continue; // loop back to start
            }
            
            


            
            // when we find an assignment...
            if(strcmp(tokenized, "AS") == 0){

                printf("creating def value!");

                getValue = true;
                continue; // loop back to start
            }
            
            }


        }





        
    }
    
    


    fclose(file);
    

}