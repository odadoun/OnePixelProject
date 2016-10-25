/* The Reader for Universe scan
 O. Dadoun & N. Darrot May 2016
 odadoun@gmail.com
 Read an ascii file (located on arduino SD card) onto SDRAM
 file (with nb_lines) format is
 pixel_i,pixel_j,Red,Green,Blue ... RGB between 0 up to 255
 180 Mega lines !!!
 Load from a file located on arduino SD card the constellation
 name and positions into memory 75 lignes
 */
#include "TheReaderUniverse.h"

TheReaderUniverse::TheReaderUniverse(): itsFileName("test1.txt"), nb_lines_read(0), nb_bytes_read(0)
{
    cout << "In the Reader Constructor" << endl;
}

TheReaderUniverse::~TheReaderUniverse()
{
    cout << "In the Reader Destructor" << endl;
}

void TheReaderUniverse::fill_sequence_online(char (&pix_rgb)[5][64])
{
    string temp = "";
    bool new_line = false;
    int line_length;
    
    if (!myFile.eof())
    {
        cout << "In fill_sequence_online " << nb_bytes_read << " --- " << nb_lines_read << endl;
        
        myFile.seekg(nb_bytes_read, myFile.beg);
        string led_list = "";
        
        while (!myFile.eof() && new_line == false)
        {
            char c;
            myFile.read(&c,1);
            if ( c != '\n'  )
            {
                temp += c;
            }
            else new_line = true;
        }
        
        line_length = temp.length() + 1;
        nb_bytes_read += line_length;
        nb_lines_read++;
        led_list = temp;
        
        /* convert the line string into pixel and rgb */
        char cool_chaine[25];
        
        strcpy(cool_chaine, led_list.c_str());
        line_extracter(cool_chaine, pix_rgb);
        new_line = false;
        temp = "";
        myFile.seekg (0, myFile.end);
        int length = myFile.tellg();
        if (nb_bytes_read >= length)
        {
            cout << "Amazing !! End of file was reached ..." << endl;
            nb_lines_read = 0;
            nb_bytes_read = 0;
            myFile.close();
            cout << itsFileName << " file closed.";
            exit(0);
        }
    }
    else
    {
        cout << "Error opening : " << itsFileName;
    }
}

void TheReaderUniverse::line_extracter(char* input_string, char (&name_value)[5][64])
{
    char *token;
    token = strtok(input_string, ",");
    int i = 1;
    while (token != NULL ) {
        sprintf(name_value[i - 1], "%s", token);
        token = strtok(NULL, ",");
        i++;
    }
}



void TheReaderUniverse::load_constellations_abacus()
{
    string temp = "";
    ifstream myFile;
    myFile.open("const.txt", std::ifstream::in);
    
    /* NAME_CONSTELLATION MIN_X MIN_Y MAX_X MAX_Y */
    cout << " Constellation abacus opened " << endl;
    
    int nb_lines = 0;

    while (!myFile.eof()) {
        
        char c ;
        myFile.read(&c,1);
        if (c != '\n' )
        {
            temp += c;
        }
        else
            if(temp!="")
        {
            //char *char_temp;
            //char_temp = (char*)malloc(temp.length() * sizeof(char));
            
            char char_temp[128];
            sprintf(char_temp, "%s", temp.c_str());
            char abac[5][64];
            line_extracter(char_temp, abac);
            
           sprintf(abacus[nb_lines].name_abac, "%s", abac[0]);
           abacus[nb_lines].begin_x = strtoul(abac[1], NULL, 0);
           abacus[nb_lines].begin_y = strtoul(abac[2], NULL, 0);
           abacus[nb_lines].end_x   = strtoul(abac[3], NULL, 0);
           abacus[nb_lines].end_y   = strtoul(abac[4], NULL, 0);
            temp = "";
            nb_lines++;
        }
    }
    myFile.close();

    cout << "File closed " << endl;
}

string TheReaderUniverse::return_constellation(unsigned long int  pix_x, unsigned long int  pix_y)
{
    unsigned long int x0 = 19000;
    unsigned long int y0 = 9500;
    
    string name_value;
    char temp[64];
    sprintf(temp, "%s", "Nothing interesting here ...");
    
    string tmp = "";
    string name = "";
    for (int i = 0; i < nb_lines_abaccus; i++)
    {
        tmp = string(abacus[i].name_abac);
        // tmp.replace(tmp.begin(), tmp.end(),"-"," ");
        
        if ( pix_x <= x0 - abacus[i].begin_x && pix_x >= x0 - abacus[i].end_x
            &&  pix_y <= y0 - abacus[i].begin_y && pix_y >= y0 - abacus[i].end_y )
        {
            name = tmp;
            //cout << name <<  " LIne " << i << endl;
            break;
        }
    }
    return name;
}

float TheReaderUniverse::GetLongitude(unsigned long int pix_x)
{
    float longitude = 0.;
    if (pix_x <= 9500) longitude = -(180. / 9500.) * (pix_x - 9500.);
    else longitude = (180. / 9499) * (pix_x - 9500.);
    return longitude;
}

float TheReaderUniverse::GetLatitude(unsigned long int pix_y)
{
    float latitude = 0.;
    if (pix_y >= 4749) latitude = (90. / 4750) * (pix_y - 4749.);
    else latitude = (90. / 4749) * (pix_y - 4749.);
    return latitude;
}


unsigned long int TheReaderUniverse::injection(unsigned long int which_line)
{
    ifstream fp;
    fp.open("test1.txt", std::ifstream::in);
    unsigned long int nb_bytes_read=0;
    char c;
    unsigned long int counter=1;
    while (!fp.eof()) {
        fp.read(&c,1);
        if(counter<=which_line)
        {
            if (c != '\n' )  nb_bytes_read+=1;
            else counter+=1;
        }
        else break;
    }
    fp.close();
    return nb_bytes_read+counter-1; // \n need to be count
}

