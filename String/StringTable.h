// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __STRINGTABLE_H__
#define __STRINGTABLE_H__

namespace core {

    template<typename t_type>
    const char* DefaultFormatString();

    class StringTable
        : public std::vector<StringVec>
    {
    public:
        std::string mColumnDelimiter;
        bool mPadLeft;

        StringTable(const char* columnDelimiter="\t");

        StringTable& startRow();

        template<typename t_type>
        StringTable& cell(const t_type& val, const char* format)
        {
            char string[256];

            snprintf(string, 256, format, val);

            addCell(string);

            return *this;
        }

        void log(Priority priority, const char* category, Logger& logger);

        void clear()
        {
            std::vector<StringVec>::clear();

            mColumnWidth.clear();
        }

    protected:
        std::vector<int> mColumnWidth;

        void addCell(const char* string);

        void updateCurrentColumnWidth(int width);
    };

} // namespace core

#endif // __STRINGTABLE_H__