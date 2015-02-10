#include <iostream>
const unsigned long long SEC = 1000L*1000L*1000L;
 
inline unsigned long long timespecTo64( const timespec &_time ) {
    return ( _time.tv_sec*SEC + _time.tv_nsec );
}
 
void runTest(clockid_t clkid, std::string const& name)
{
    timespec t;
    int ctr = 0;
    timespec begin;
    clock_gettime ( clkid , &begin);
    unsigned long long startTime = timespecTo64(begin);
    t = begin;
    while (timespecTo64(t) - startTime < 2*SEC)
    {
        clock_gettime( clkid, &t);
        ctr++;
    }
    unsigned long long nsDiff = timespecTo64(t) - startTime;
    std::cout << std::endl;
    std::cout << ctr << " executions of " << name << " in " << (nsDiff*1.0)/SEC << " seconds; " << (nsDiff*1.0/ctr) << " ns per execution" << std::endl;
    clock_getres( clkid, &t);
    std::cout << "Resolution is " << timespecTo64(t) << std::endl;
    std::cout << std::endl;
}
 
void runClockTest()
{
    int ctr = 0;
    clock_t startTime = clock();
    clock_t curTime = startTime;
    while ((curTime - startTime) < 2L*CLOCKS_PER_SEC)
    {
        curTime = clock();
        ctr++;
    }
    double diff = (curTime - startTime)*1.0/CLOCKS_PER_SEC;
    std::cout << std::endl;
    std::cout << ctr << " executions of clock() in " << diff << " seconds; " << (SEC*diff/ctr) << " ns per execution" << std::endl;
    std::cout << std::endl;
}
 
#define TIME_TEST(CLKID) runTest(CLKID,#CLKID)
 
int main()
{
    //runClockTest();
    TIME_TEST(CLOCK_REALTIME);
    TIME_TEST(CLOCK_MONOTONIC);
    TIME_TEST(CLOCK_PROCESS_CPUTIME_ID);
    TIME_TEST(CLOCK_THREAD_CPUTIME_ID);
    return 0;
}