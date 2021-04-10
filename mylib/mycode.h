#ifndef MYCODEH
#define MYCODEH

template<typename T>
void in_place_switch(T& a, T& b) {
    T temp_T = a;
    a = b;
    b = temp_T;
}

void init_log(const char* logfilename) {
    freopen(logfilename, "w", stderr);
    time_t now = time(0);
    struct tm  tstruct;
    char  buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    clog<<string(buf)<<": program start"<<'\n';
}

#endif