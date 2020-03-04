#include <iostream>
#include <fstream>
#include <string>

void makni_razmake_s_pocetka(std::string *s)
{
    while((*s)[0] == ' ') *s = s->substr(1);
}

void makni_razmake_s_kraja(std::string *s)
{
    while((*s)[s->size() - 1] == ' ') (*s) = s->substr(0, s->size() - 1);
}

int main()
{
    int first_word = 0; //Za razlikovanje linija s puno razmaka
    std::string new_line, line;
    std::ifstream in_file("input.txt"); //otvaranje input datoteke
    if(!in_file.is_open())
    {
        std::cout << "Greska prilikom otvaranja input datoteke." << std::endl;
        return 1;
    }
    
    std::ofstream out_file("output.txt"); //otvaranje output datoteke
    if(!out_file.is_open())
    {
        std::cout << "Greska prilikom otvaranja output datoteke" << std::endl;
        return 2;
    }

    int num = 0; //brojac paragrafa tako da znam isprintati endl prije novog osim prije prvog (samo za num = 1 printa)
    int prva_isprintana = 0; //ovo ce mi sluziti za provjeru kad dodajem endl a kada ne (da na kraju ne prebacim nepotrebno u novu liniju) 
    while(!in_file.eof()) //učitavanje paragrafa
    {
        getline(in_file, new_line);

        for(first_word = 0; first_word < new_line.size(); ++first_word)
            if(new_line[first_word] != ' ') break;

        if(first_word == new_line.size() || new_line.empty()) //linija puna razmaka = prazna linija
        {
            out_file << std::endl;
            continue;
        }
        
        ++num; //ovdje jer prazne linije ne brojim kao paragrafe
        while((!new_line.empty()) && (!in_file.eof())) //stvaranje paragrafa
        {
            
            line += new_line;
            line += " ";
            
            getline(in_file, new_line);
            if((!new_line.empty()) && (!in_file.eof()))
            {
                for(first_word = 0; first_word < new_line.size(); ++first_word)
                    if(new_line[first_word] != ' ') break;
                
                if(first_word == new_line.size()) //linija puna razmaka = prazna linija
                    break;
            }
        }
        if(in_file.eof()){ line += new_line; line += " "; }

        //UREDIVANJE PARAGRAFA
        makni_razmake_s_kraja(&line);
        int first_space; //treba mi kod micanja suvisnih razmaka

        for(first_word = 0; first_word < line.size(); ++first_word)
            if(line[first_word] != ' ') break;

        if(first_word >= 72) //Puno razmaka na pocetku koji su veci od cijele jedne linije
        {
            line = line.substr(first_word);
            out_file << std::endl;
            first_word = 0;
        }
        //Uredivanje paragrafa radi dobro!!!

        //ISPISIVANJE PARAGRAFA
        if(num > 1) out_file << std::endl;

        while(!line.empty())
        {
            if(line.size() <= 72) //linija manje od ili tocno 72 znaka pa se moze printat
            {
                if(prva_isprintana)
                    out_file << std::endl;
                
                out_file << line;
                ++prva_isprintana;

                line.clear();
            }

            else{
                first_space = line.find(' ');

                if(first_space == std::string::npos) //nema razmaka pa je linija samo jedna rijec i ispisujemo ju
                {
                    if(prva_isprintana)
                        out_file << std::endl;

                    out_file << line;
                    ++prva_isprintana;

                    line.clear();
                }

                else if(first_space >= 72) //imamo rijec dugu ili duzu od 72 znaka pa ju ispisujemo zasebno u red
                {
                    if(prva_isprintana)
                        out_file << std::endl;
                    
                    out_file << line.substr(0, first_space);
                    line = line.substr(first_space + 1);
                    ++prva_isprintana;
                }

                else //prelamamo liniju
                {
                    std::string tmp1, tmp2;          
                    if(line[72] == ' ') //prelomili na razmaku pa moramo ukloniti suvisne razmake na kraju gornje i pocetku donje linije
                    {
                        if(line[71] == ' ' || line[73] == ' ') //ima suvisnih razmaka pa ih se mice i ponovno ulazi u petlju
                        {
                            tmp1 = line.substr(0, 72);
                            makni_razmake_s_kraja(&tmp1);
                            tmp2 = line.substr(73);
                            makni_razmake_s_pocetka(&tmp2);
                            line = tmp1 + " " + tmp2;
                            continue;
                        }

                        if(prva_isprintana)  //to je jedini razmak izmedu dvije rijeci pa na njemu lomimo
                            out_file << std::endl;
                        
                        out_file << line.substr(0, 72);
                        line = line.substr(73);
                        ++prva_isprintana;
                        continue;
                    }

                    else //prelomili na iducoj recenici pa micemo suvisne razmake ili ju prebacujemo u iduci red
                    {
                        tmp1 = line.substr(0, 72);
                        int last_space = tmp1.rfind(' ');
                        tmp1 = tmp1.substr(0, last_space);
                        makni_razmake_s_kraja(&tmp1);
                        tmp2 = line.substr(last_space + 1);
                        int word_length = tmp2.find(' ');

                        if(tmp1.size() + word_length < 72) //provjerava spajamo li ponovno liniju ili ne (ako ima visak razmaka, oni se nakon loma micu)
                        {
                            line = tmp1 + " " + tmp2;
                            continue;
                        }

                        if(prva_isprintana) //ne spajamo ih nego ispisujemo
                            out_file << std::endl;

                        out_file << tmp1;
                        line = tmp2;
                        ++prva_isprintana;                     
                    }
                }
            }
        }
    }

    in_file.close();
    out_file.close();
    return 0;
}