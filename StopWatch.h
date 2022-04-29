#ifndef STOP_WATCH
#define STOP_WATCH

#include <chrono>
#include <vector>
#include <string>

namespace stopwatch
{
    enum class TimeFormat { NANOSECONDS, MICROSECONDS, MILLISECONDS, SECONDS };
    using HighResClock = std::chrono::high_resolution_clock;
    using time_pt = std::chrono::time_point<HighResClock>;

    class StopWatch
    {
    public:
        StopWatch() : m_startTime(), m_laps({}) 
        {
            start();
        }

        void start()
        {
            m_startTime = HighResClock::now();
            m_laps = { m_startTime };
        }

        template<TimeFormat tmfmt>
        std::uint64_t lap()
        {
            const auto timeNow = HighResClock::now();
            const auto lastResetTime = m_laps.back();
            m_laps.push_back(timeNow);
            return ticks<tmfmt>(lastResetTime, timeNow);
        }

        template<TimeFormat tmfmt>
        std::uint64_t elapsed()
        {
            const auto timeNow = HighResClock::now();
            return ticks<tmfmt>(m_startTime, timeNow);
        }

        template<TimeFormat fmt_total, TimeFormat fmt_lap = fmt_total>
        std::pair<std::uint64_t, std::vector<std::uint64_t>> elapsed_laps() 
        {
            std::vector<std::uint64_t> lap_times;
            lap_times.reserve(m_laps.size() - 1);

            for (std::size_t idx = 0; idx <= m_laps.size() - 2; ++idx) 
            {
                const auto lap_end = m_laps[idx + 1];
                const auto lap_start = m_laps[idx];
                lap_times.push_back(ticks<fmt_lap>(lap_start, lap_end));
            }

            return { ticks<fmt_total>(m_startTime, m_laps.back()), lap_times };
        }

    private:
        time_pt m_startTime;
        std::vector<time_pt> m_laps;

        template<TimeFormat tmfmt/* = TimeFormat::MILLISECONDS*/>
        static std::uint64_t ticks(const time_pt& startTime, const time_pt& endTime)
        {
            const auto duration = endTime - startTime;
            const auto nanoSecCnt = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
            const auto decimalUp = [ns_count = nanoSecCnt](const TimeFormat& frmt)
            {
                switch (frmt)
                {
                    case TimeFormat::MICROSECONDS:
                    {
                        std::uint64_t up = ((ns_count / 100) % 10 >= 5) ? 1 : 0;
                        return up;
                    }
                    case TimeFormat::MILLISECONDS:
                    {
                        std::uint64_t up = ((ns_count / 1'000'00) % 10 >= 5) ? 1 : 0;
                        return up;
                    }
                    case TimeFormat::SECONDS:
                    {
                        std::uint64_t up = ((ns_count / 1'000'000'00) % 10 >= 5) ? 1 : 0;
                        return up;
                    }
                    default:
                    {
                        return uint64_t(0);
                    }
                }
            };

            switch (tmfmt)
            {
                case TimeFormat::NANOSECONDS:
                {
                    return nanoSecCnt;
                }
                case TimeFormat::MICROSECONDS:
                {
                    const auto mus_count = (nanoSecCnt / 1000) + decimalUp(tmfmt);
                    return mus_count;
                }
                case TimeFormat::MILLISECONDS:
                {
                    const auto ms_count = (nanoSecCnt / 1'000'00) + decimalUp(tmfmt);
                    return ms_count;
                }
                case TimeFormat::SECONDS:
                {
                    const auto sec_count = (nanoSecCnt / 1'000'000'00) + decimalUp(tmfmt);
                    return sec_count;
                }
            }
            return uint64_t(0);
        }
    };

    constexpr TimeFormat ns = TimeFormat::NANOSECONDS;
    constexpr TimeFormat mus = TimeFormat::MICROSECONDS;
    constexpr TimeFormat ms = TimeFormat::MILLISECONDS;
    constexpr TimeFormat s = TimeFormat::SECONDS;

    constexpr TimeFormat nanoseconds = TimeFormat::NANOSECONDS;
    constexpr TimeFormat microseconds = TimeFormat::MICROSECONDS;
    constexpr TimeFormat milliseconds = TimeFormat::MILLISECONDS;
    constexpr TimeFormat seconds = TimeFormat::SECONDS;


    std::string show_times(const std::vector<std::uint64_t>& times) 
    {
        std::string result("{");
        for (const auto& t : times)
            result += std::to_string(t) + ",";

        result.back() = static_cast<char>('}');
        return result;
    }
}
#endif // !STOP_WATCH

