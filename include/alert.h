#ifndef ALERT_H_
#define ALERT_H_

  struct Alert_type {
    time_t        start;
    time_t        end;
    const char*   title;
    const char*   tag;
  };

#endif