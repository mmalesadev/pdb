#pragma once
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <fstream>
#include <string>

namespace Pdb
{

using boost::property_tree::ptree;

class Config
{
public:
    static Config& getInstance()
    {
        static Config* instance = new Config();
        return *instance;
    }

private:
    Config();

public:
    float masterVolumeForAwsSynthesized;
    float masterVolumeForAudiobooks;

private:
    ptree pt_;
};

}