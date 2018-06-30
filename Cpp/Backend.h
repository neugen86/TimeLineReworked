#pragma once

#include <Cpp/Controls/Common.h>

namespace backend
{
class Simulator : public IDataProvider<NamedIntervalList>
{
    const int m_count = 0;
    const TimeInterval m_interval;

public:
    Simulator(unsigned count);

    const TimeInterval& interval() const { return m_interval; }

public: // IDataProvider
    NamedIntervalList getData() override;

};

} // namespace backend
