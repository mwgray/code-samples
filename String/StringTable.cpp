// Copyright 2006-12 HumaNature Studios Inc.

#include "CorePch.h"

#include "StringTable.h"

#include "Math/MathUtil.h"

namespace core {
    template<> const char* DefaultFormatString<const char*>() { static const char* format = "%s"   ; return format; }
    template<> const char* DefaultFormatString<int        >() { static const char* format = "%d"   ; return format; }
    template<> const char* DefaultFormatString<float      >() { static const char* format = "%2.2f"; return format; }

    void padLeftTo(std::string &str, const size_t targetWidth, const char paddingChar = ' ')
    {
        if(targetWidth > str.size())
            str.insert(0, targetWidth - str.size(), paddingChar);
    }

    StringTable::StringTable(const char* columnDelimiter)
        : mColumnDelimiter(columnDelimiter)
        , mPadLeft(false)
    {
    }

    StringTable& StringTable::startRow()
    {
        push_back(StringVec());

        return *this;
    }

    void StringTable::log(Priority priority, const char* category, Logger& logger)
    {
        for(size_t i = 0; i < mColumnWidth.size(); ++i)
        {
//            cell(mColumnWidth[i], "%d,");
        }

        for(auto rowIter = begin(); rowIter != end(); ++rowIter)
        {
            std::string rowString;
            int i = 0;
            for (auto cellIter = (*rowIter).begin(); cellIter != (*rowIter).end(); ++cellIter)
            {
                int width = mColumnWidth[i++];

                std::string cellString = *cellIter;

                if(mPadLeft)
                {
                    padLeftTo(cellString, width);
                }

                rowString += cellString;

//                if(&logger != &ScreenReporter::Instance())
                {
                    // only use tabs when console logging
                    rowString += mColumnDelimiter;
                }
            }

            logger.log(priority, category, rowString.c_str());
        }
    }

    void StringTable::addCell(const char* string)
    {
        int length = strlen(string);

        StringVec& currentRow = back();

        currentRow.push_back(string);

        while(mColumnWidth.size() < currentRow.size())
        {
            mColumnWidth.push_back(length);
        }

        updateCurrentColumnWidth(length);
    }

    void StringTable::updateCurrentColumnWidth(int width)
    {
        int currentWidth = mColumnWidth[back().size() - 1];

        mColumnWidth[back().size() - 1] = MathUtil::max(currentWidth, width);
    }
} // namespace core