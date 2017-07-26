/* The MIT License (MIT)
*
* Copyright (c) 2017 Andrew Yeung/Bhavya Kashyap <ab.deveng@gmail.com>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE. */

#pragma once

#include <stdint.h>

#include <abdeveng/core/service/logger.h>
#include <abdeveng/core/test/common/evaluator.h>

template<typename DUT_TYPE>
class CTestCase;

template<typename DUT_TYPE>
class CTestHarness
{
public:
    CTestHarness(DUT_TYPE&, ILogger&);
    virtual ~CTestHarness();

    virtual void Run() final;

protected:
    virtual void ResetDUT()           = 0;
    virtual uint8_t GetNumTestCases() = 0; // TODO HACK: Limited to 256 test cases
    virtual CTestCase<DUT_TYPE> GetTestCase(uint8_t testIndex, CEvaluator& evaluator) = 0;

    // Helpers: ILogger
    virtual void LogEol() final;
    virtual void Log(const char[], bool eol = true) final;
    virtual void Log(const int, ILogger::FORMAT = ILogger::FORMAT::DECIMAL, bool eol = true) final;
    virtual void Log(const long, ILogger::FORMAT = ILogger::FORMAT::DECIMAL, bool eol = true) final;
    virtual void Log(const unsigned int, ILogger::FORMAT = ILogger::FORMAT::DECIMAL, bool eol = true) final;
    virtual void Log(const unsigned long, ILogger::FORMAT = ILogger::FORMAT::DECIMAL, bool eol = true) final;

private:
    // Disable copying and assignments
    CTestHarness(const CTestHarness&);
    CTestHarness& operator=(const CTestHarness&);

    class CDelCallbackValidation : public CEvaluator::IDelCallbackValidation
    {
    public:
        CDelCallbackValidation(ILogger&);
        virtual ~CDelCallbackValidation();

        // CEvaluator::IDelCallbackValidation
        virtual void OnValidationFail(bool expected, bool actual);
        virtual void OnValidationFail(int8_t expected, int8_t actual);
        virtual void OnValidationFail(int16_t expected, int16_t actual);
        virtual void OnValidationFail(int32_t expected, int32_t actual);
        virtual void OnValidationFail(const CEvaluator::EVALUATION<bool>& evaluation);
        virtual void OnValidationFail(const CEvaluator::EVALUATION<int8_t>& evaluation);
        virtual void OnValidationFail(const CEvaluator::EVALUATION<int16_t>& evaluation);
        virtual void OnValidationFail(const CEvaluator::EVALUATION<int32_t>& evaluation);

    private:
        ILogger& m_logger;
    };

    CDelCallbackValidation m_del_callback_validation;
    CEvaluator m_evaluator;

    ILogger& m_logger;

    DUT_TYPE& m_dut;
};