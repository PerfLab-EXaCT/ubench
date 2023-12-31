#include <time.h> 
#include <unistd.h>
                  
enum { NS_PER_SECOND = 1000000000 };   
                                      
void print_time(char *str_log, struct timespec t1, struct timespec t2)  
{                                                                      
        struct timespec td;                                           
        td.tv_nsec = t2.tv_nsec - t1.tv_nsec;                        
        td.tv_sec  = t2.tv_sec - t1.tv_sec;                         
        if (td.tv_sec > 0 && td.tv_nsec < 0)                       
        {                                                         
                td.tv_nsec += NS_PER_SECOND;                     
                td.tv_sec--;                                    
        }                                                      
        else if (td.tv_sec < 0 && td.tv_nsec > 0)             
        {                                                    
                td.tv_nsec -= NS_PER_SECOND;                
                td.tv_sec++;                               
        }                                                 
        printf(" %s %d.%.9ld\n", str_log, (int)td.tv_sec, td.tv_nsec);  
}      
