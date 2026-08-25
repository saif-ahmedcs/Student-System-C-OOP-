#ifndef TABLE_PRINTER_H
#define TABLE_PRINTER_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

inline void printTableDivider(const std::string& color, int width, char ch = '-') {
    std::cout << color;
    for (int i = 0; i < width; i++) {
        std::cout << ch;
    }
    std::cout << "\033[0m\n";
}

inline void printTableCell(const std::string& color, int width) {
    std::cout << color << "|\033[0m " << std::left << std::setw(width);
}

struct TableColumn {
    std::string header;
    int width;
};

inline void printTableHeader(const std::string& color, int dividerWidth, const std::vector<TableColumn>& columns) {
    printTableDivider(color, dividerWidth);
    for (int i = 0; i < (int)columns.size(); i++) {
        std::cout << color << "|\033[0m " << std::left << std::setw(columns[i].width) << columns[i].header;
    }
    std::cout << color << "|\033[0m\n";
    printTableDivider(color, dividerWidth);
}

inline void printTableRowEnd(const std::string& color) {
    std::cout << color << "|\033[0m\n";
}

inline void printEmptyTableNotice(const std::string& color, int width, const std::string& message) {
    std::cout << message << "\n";
    printTableDivider(color, width, '=');
}

#endif
