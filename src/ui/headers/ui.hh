#pragma once

#include <vector>
#include <list>
#include <thread>
#include <algorithm>

#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/screen.hpp"

#include "assembler.hh"
#include "ram.hh"
#include "cpu.hh"
#include "loader.hh"
#include "helpers.hh"
#include "padding.hh"

#define SCREEN_HEIGHT 30
#define STACK_HEIGHT 30
#define STACK_BYTES_PER_LINE 4
#define MEMORY_BYTES_PER_LINE 18
#define REGISTERS_PER_LINE 5
#define ADDRESS_WIDTH 8
#define INSTRUCTIONS_RANGE 10

class UserInterface {
private:
    std::string code;
    bool interactive;
    int speed;

    Assembler assembler;
    RandomAccessMemory ram;
    CentralProcessingUnit cpu;
    Loader loader;

    Program program;

    std::string reset_position;

    std::unique_ptr<ftxui::Node> render_stack();
    std::unique_ptr<ftxui::Node> render_data_segment();
    std::unique_ptr<ftxui::Node> render_registers();
    std::unique_ptr<ftxui::Node> render_instructions();
    std::unique_ptr<ftxui::Node> render_fpu();
    std::unique_ptr<ftxui::Node> render_sse();

public:
    UserInterface() = delete;
    UserInterface(std::string const & code, bool interactive, int speed);

    void start();
    void render();
};
