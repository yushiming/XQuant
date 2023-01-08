//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  StringHelper.h
//
//   Description:  字符串辅助处理功能函数集
//
//       Version:  1.0
//       Created:  2023年1月3日 13时54分28秒
//      Revision:  none
//      Compiler:  vs2019
//
//        Author:  yushiming
//  Organization:  xxy 
//
//=====================================================================================

#pragma once

#include <string>
#include <vector>


namespace XQuant{

    class StringHelper {
    public:
        /*
         * @desc Split string `str` with `sep` into substrings.
         * @param str The source string to be split.
         * @param sep The seperator character set.
         * @param limit The maximun count of substrings. 0 or negative means no limitation.
         * @return a vector contains substrings.
         */
        static std::vector<std::string> split(const std::string& str,
            const std::string& sep,
            int limit);

        /*
         * @desc Split string `str` with `sep` into substrings.
         * @param str The source string to be split.
         * @param sep The seperator character set.
         * @param limit The maximun count of substrings. 0 or negative means no limitation.
         * @param callback The callback that will be called when a substring is found.
         * @return the number of substrings.
         */
        static size_t split(const std::string& str,
            const std::string& sep,
            int limit,
            const std::function<void(const std::string&, int)>& callback);

        /*
         * @desc Trim the beginning blank characters of string `str`.
         * @param str The source string to trimmed.
         * @return the trimmed string.
         */
        static std::string ltrim(const std::string& str);

        /*
         * @desc Trim the ending blank characters of string `str`.
         * @param str The source string to trimmed.
         * @return the trimmed string.
         */
        static std::string rtrim(const std::string& str);

        /*
         * @desc Trim both the beginning and ending blank characters of string `str`.
         * @param str The source string to trimmed.
         * @return the trimmed string.
         */
        static std::string trim(const std::string& str);


        /*
        *  @desc Gets the md5 hash for the given string.
        *  @param data The string to calculate md5 hash.
        *  @return The md5 hash for the data
        */
        //static std::string getStringMD5Hash(const std::string& data);

        static std::string concat(const std::vector<std::string>& strArr, const std::string& sep);

        inline static std::string boolToString(const bool b) {
            return b ? "true" : "false";
        }

        inline static bool boolFromString(const std::string& str) {
            if (str == "true" || str == "True" || str == "TRUE") {
                return true;
            }
            return false;
        }

    };
}