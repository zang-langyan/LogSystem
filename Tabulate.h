#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <algorithm>

enum class Align { Left, Right, Center };
enum class TableStyle { ASCII, Unicode };

template<typename T>
static std::string TtoString(const T& val) {
    std::ostringstream oss;
    oss << val;
    return oss.str();
}

class Tabulate {
public:
    std::string to_string() {
        std::stringstream ss;
        print(ss);
        return ss.str();
    }
    
    void setHeaders(const std::vector<std::string>& headerRow, const std::vector<Align>& aligns = {Align::Center, Align::Center, Align::Center}) {
        headers = headerRow;
        alignments = aligns;
        updateColumnWidths(headers);
    }
    
    
    template<typename... Args>
    void addRow(Args&&... args) {
        std::vector<std::string> row = { TtoString(std::forward<Args>(args))... };
        addRowv(row);
    }
    
    void addRowv(const std::vector<std::string>& row) {
        rows.push_back(row);
        updateColumnWidths(row);
    }
    
    void setStyle(TableStyle s) {
        style = s;
    }
    
    void print(std::ostream& os = std::cout) const {
        printBorder(os, true);
        if (!headers.empty()) {
            printRow(os, headers);
            printDivider(os);
        }
        for (const auto& row : rows)
        printRow(os, row);
        printBorder(os, false);
    }

private:
    std::string formatCell(const std::string& text, size_t width, Align align) const {
        if (text.length() >= width) return text;

        size_t space = width - text.length();
        switch (align) {
            case Align::Left:
                return text + std::string(space, ' ');
            case Align::Right:
                return std::string(space, ' ') + text;
            case Align::Center:
                return std::string(space / 2, ' ') + text + std::string((space + 1) / 2, ' ');
        }
        return text;
    }

    void updateColumnWidths(const std::vector<std::string>& row) {
        if (columnWidths.size() < row.size())
            columnWidths.resize(row.size(), 0);
        for (size_t i = 0; i < row.size(); ++i)
            columnWidths[i] = std::max(columnWidths[i], row[i].length());
    }

    void printDivider(std::ostream& os) const {
        std::string left, mid, right;
        char bar {'-'};
        if (style == TableStyle::Unicode) {
            left = "├"; mid = "┼"; right = "┤";
        } else {
            left = "+"; mid = "+"; right = "+";
        }

        os << left;
        for (size_t i = 0; i < columnWidths.size(); ++i) {
            os << std::setfill(bar) << std::setw(columnWidths[i] + 2) << "";
            os << std::setfill(' ') << (i + 1 < columnWidths.size() ? mid : right);
        }
        os << "\n";
    }

    void printBorder(std::ostream& os, bool top) const {
        std::string left, mid, right;
        char bar {'-'};
        if (style == TableStyle::Unicode) {
            left = top ? "┌" : "└";
            mid  = top ? "┬" : "┴";
            right = top ? "┐" : "┘";
        } else {
            left = "+"; mid = "+"; right = "+";
        }

        os << left;
        for (size_t i = 0; i < columnWidths.size(); ++i) {
            os << std::setfill(bar) << std::setw(columnWidths[i] + 2) << "";
            os << std::setfill(' ') << (i + 1 < columnWidths.size() ? mid : right);
        }
        os << "\n";
    }

    void printRow(std::ostream& os, const std::vector<std::string>& row) const {
        os << (style == TableStyle::Unicode ? "│" : "|");
        for (size_t i = 0; i < columnWidths.size(); ++i) {
            std::string cell = (i < row.size()) ? row[i] : "";
            Align a = (i < alignments.size()) ? alignments[i] : Align::Left;
            os << " " << formatCell(cell, columnWidths[i], a) << " ";
            os << (style == TableStyle::Unicode ? "│" : "|");
        }
        os << "\n";
    }

private:
    std::vector<std::string> headers;
    std::vector<Align> alignments;
    std::vector<std::vector<std::string>> rows;
    std::vector<size_t> columnWidths;
    TableStyle style = TableStyle::ASCII;
};