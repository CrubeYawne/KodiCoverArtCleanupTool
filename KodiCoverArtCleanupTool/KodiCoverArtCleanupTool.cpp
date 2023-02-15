// KodiCoverArtCleanupTool.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include <string>
#include <regex>
#include <fstream>

#include <thread>
#include <chrono>

#include <curl/curl.h>

/// <summary>
/// Store the response from a web request to string
/// </summary>
/// <param name="data"></param>
/// <param name="size"></param>
/// <param name="nmemb"></param>
/// <param name="userp"></param>
/// <returns>Size of resulting string</returns>
static size_t writeToString(void* data, size_t size, size_t nmemb, void* userp)
{
    
    size_t realsize = size * nmemb;
    /*
    * //commented out because we don't actually need the body of the response, but we don't want the console to get it either
    std::string* str = static_cast<std::string*>(userp);
    str->append(static_cast<char*>(data), realsize);
    */
    
    return realsize;
}

/// <summary>
/// checks if a thumbnail element is still valid
/// </summary>
/// <param name="thumbnail_url">xml element with http url inside text</param>
/// <returns>true if thumbnail still exists</returns>
bool checkThumbnail(const std::string_view thumbnail_url)
{
    bool return_value = false;

    std::cout << "\n\tChecking " << thumbnail_url << '\n';

    //remove intro/outro xml element tags
    auto start_chev = thumbnail_url.find(">");
    auto end_chev = thumbnail_url.find("<", start_chev);

    //create a string to store the cleaned url
    std::string full_url{ thumbnail_url.substr(start_chev + 1, end_chev - start_chev - 1) };

    std::cout << "\tCleaned: [" << full_url <<  "] " << full_url.size() << '\n';

    CURL* curl;

    curl = curl_easy_init();

    if (curl)
    {
        

        std::string remote_file_content;

        curl_easy_setopt(curl, CURLOPT_URL, full_url);

        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeToString);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &remote_file_content);        

        auto res = curl_easy_perform(curl);

        long http_result_code{ 0 };

        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_result_code);

        std::cout << "\tHTTP CODE: " << http_result_code << '\n';

        if (res != CURLE_OK)//if the curl request worked properly
        {
            std::cout << "*******************" << '\n';
            std::cout << res << '\n';
            std::cout << "*******************" << '\n';            
        }
        else
        {
            if (http_result_code == 200)//if the request came back 200, it's a good image
            {
                return_value = true;
            }
        }

        curl_easy_cleanup(curl);

    }


    //sleep to avoid destroying servers
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    return return_value;
}


/// <summary>
/// Simple replace function. replaces based on found index + length
/// </summary>
/// <param name="str">string to perform action on</param>
/// <param name="from">string to search for</param>
/// <param name="to">string to replace with</param>
/// <returns></returns>
bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}


/// <summary>
/// check an NFO for bad thumbnail images
/// </summary>
/// <param name="target_file">xml element with value being http url</param>
/// <returns>true if the file contains at least 1 good thumbnail</returns>
bool checkNFO(const std::string_view target_file, bool rewrite_file = false)
{

    bool nfo_return = false;

    std::cout << "\tChecking file: " << target_file;

    //regex to find thumbnail tags
    std::regex thumb_search_regex("<thumb aspect=\"poster\" [^<]+>[^<]+<\\/thumb>");

    std::ifstream nfoFile(target_file);

    std::string file_contents{ "" };//full file input

    std::string line_output{ "" };//chunk input

    while (std::getline(nfoFile, line_output))
    {
        file_contents += line_output;
    }

    nfoFile.close();

    //if nfo file contains any matching thumbnail sections
    if (std::regex_search(file_contents, thumb_search_regex))
    {
        
        bool has_file_changed = false;

        std::string new_file_data{ file_contents };

        std::cout << "\n\tContains thumbnails";

        auto thumb_begin = std::sregex_iterator(file_contents.begin(), file_contents.end(), thumb_search_regex);
        auto thumb_end = std::sregex_iterator();

        bool has_good_thumbnails = false;

        for (std::sregex_iterator i = thumb_begin; i != thumb_end && !has_good_thumbnails; ++i)
        {
            std::smatch match = *i;

            auto thumb_result = checkThumbnail(match.str());

            //if thumbnail is good, flag it as healthy
            if (thumb_result)
            {
                std::cout << "\tHas good thumbnail";
                has_good_thumbnails = true;
            }
            else
            {
                //if thumbnail not healthy replace in nfo file
                auto replace_result = replace(new_file_data, match.str(), "");

                if (!replace_result)
                    std::cout << "\tCouldn't replace thumbnail url" << '\n';

                if (new_file_data == file_contents)
                    std::cout << "\tReplacement Issue" << '\n';
                   

                has_file_changed = true;
            }

        }
        
        nfo_return = has_good_thumbnails;


        if (has_file_changed && rewrite_file)
        {
            std::cout << "\n\tWriting new NFO" << '\n';
            std::ofstream newNfoFile(target_file, std::ofstream::trunc);
            newNfoFile << new_file_data;
            newNfoFile.close();
        }

    }
    else
    {
        std::cout << "\n\tNo thumbnails";
    }

    

    return nfo_return;
}

bool checkParam(int argc, char* argv[], const char parameter_name[2])
{
    auto has_parameter = false;

    for (auto i = 0; i != argc && !has_parameter; ++i)
    {
        //std::cout << (strcmp(argv[i], parameter_name) == 0) << argv[i] << parameter_name << '\n';
        has_parameter = (strcmp(argv[i],parameter_name) == 0);
    }

    return has_parameter;
}


int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: KodiCoverArtCleanupTool target_directory -e -d" << '\n';
        std::cout << "\t-e\tRemove dead thumbnail links" << '\n';
        std::cout << "\t-d\tRemove files without any thumbnail" << '\n';
        std::cout << "no parameters to run as test" << '\n';
        return 0;
    }

    auto remove_dead_links = checkParam(argc, argv, "-e");
    auto remove_dead_files = checkParam(argc, argv, "-d");

    //auto remove_dead_links = false;
    //auto remove_dead_files = false;

    if (remove_dead_files)
        std::cout << "REMOVE DEAD FILES (-d) ENABLED" << '\n' << '\n';

    if (remove_dead_links)
        std::cout << "REMOVE DEAD LINKS (-e) ENABLED" << '\n' << '\n';

    char* target_directory = argv[1];

    using dir_search = std::filesystem::recursive_directory_iterator;

    std::vector<std::string> reviewList{};

    std::cout << "Checking target directory [" << target_directory << "]" << '\n';

    for (const auto& dirEntry : dir_search(target_directory))
    {
        std::string fname_s{ dirEntry.path().generic_string() };
        
        if (fname_s.substr(fname_s.size() - 3, 3) == "nfo")//filer by NFO files
        {
            reviewList.push_back(fname_s);
            
        }
        
    }

    std::cout << "Found " << reviewList.size() << " items" << '\n';

    std::vector<std::string> removeList{};

    for (int i = 0; i != reviewList.size(); ++i)
    {
        std::cout << "\n\nLoop: " << i << "/" << reviewList.size() - 1 << '\n';
        auto fname = reviewList[i];

        auto check_result = checkNFO(fname, remove_dead_links);

        if (!check_result)//if item has no good thumbnail URLs in it add to delete list
            removeList.push_back(fname);
    }
    

    std::cout << "\n\nREMOVING " << removeList.size() << ":" << '\n';

    if (removeList.size() == 0)
        std::cout << "\tNothing";

    //delete all "bad" nfo files
    for (const auto& removeItem : removeList)
    {
        std::cout << '\t' << removeItem;

        if (remove_dead_files)
        {
            auto removeResult = std::remove(removeItem.c_str());

            std::cout << " removed: " << (removeResult == 0 ? "OK" : "FAIL");
        }

        std::cout << '\n';
    }

    return 0;

}
