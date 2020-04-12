#pragma once

class Timestep
{
public:
   Timestep(double t) : m_time(t) {}

   double GetSeconds() { return m_time; }
   double GetMilliSeconds() { return 1000 * m_time; }

   operator double() { return m_time; }

private:
   double m_time;
};