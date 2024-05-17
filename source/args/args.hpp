#pragma once
#ifndef ARGS_HPP
#define ARGS_HPP

#include "config.hpp"
#include "templates.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>

namespace mtv {
class Args {
  public:
    Args() = default;
    Args(int argc, char *argv[]);
    void recon();
    bool should_exit() const { return exit; }
    std::string get_output_file() const { return output_file; }
    std::string get_input_file() const { return input_file; }

  private:
    std::string get();
    std::vector<std::string> args;
    std::string input_file;
    std::string output_file;
    int index{0};
    bool exit;
    bool input_file_set{false};

    void process_output(const std::string &val);
    void process_input(const std::string &val);
    void process_version();
    void process_help();
    static std::filesystem::path get_current_path();
};
} // namespace mtv

#endif // ARGS_HPP