#include "types_p2.h"
#include "utils.h"



void Person::set_gender(int data) { gender = data; }
int Person::get_gender(void)      { return gender; }

void Person::set_order(unsigned long data) { order = data; }
unsigned long Person::get_order(void)      { return order; }

void Person::set_use_order(unsigned long data) { use_order = data; }
unsigned long Person::get_use_order(void)      { return use_order; }

void Person::set_time(long data) { time_to_stay_ms = data; }
long Person::get_time() {return time_to_stay_ms;}
int Person::ready_to_leave(void) {
    struct timeval t_curr;
    gettimeofday(&t_curr, NULL);

    if (get_elasped_time(t_start, t_curr) >= time_to_stay_ms) { return 1; }
    else { return 0; }
}




void Person::start(void) {
    gettimeofday(&t_start, NULL);
    //printf("(%lu)th person enters the fittingroom: \n", order);
    //printf(" - (%lu) milliseconds after the creation\n", get_elasped_time(t_create, t_start));
}

void Person::complete(void) {
    gettimeofday(&t_end, NULL);
    printf("(%lu)th person comes out of the fittingroom: \n", order);
    printf(" - (%lu) milliseconds after the creation\n", get_elasped_time(t_create, t_end));
    printf(" - (%lu) milliseconds after using the fittingroom\n", get_elasped_time(t_start, t_end));
}

Person::Person() {
    gettimeofday(&t_create, NULL);
}



// You need to use this function to print the Fittingroom's status
void Fittingroom::print_status(void) {
    printf("Print fittingroom status\n");
}


// Call by reference
// This is just an example. You can implement any function you need
void Fittingroom::add_person(Person& p) {
    // Do nothing;
    p.start();
    users.push_back(p);
    if (p.get_gender() == 0)
    {
        status = MENPRESENT;
        rmst.man++;
    }
    else
    {
        status = WOMENPRESENT;
        rmst.woman++;
    }
    rmst.total++;
    if (rmst.total == size)
        status = FULL;
}

void Fittingroom::remove_person(int i) {
    if (users[i].get_gender())
        rmst.woman--;
    else
        rmst.man--;
    users.erase(users.begin()+i);
    rmst.total--;
    if (rmst.total == 0)
        status = EMPTY;
}

void Fittingroom::set_size(int s) {
    size = s;
    rmst.total = 0;
    rmst.man = 0;
    rmst.woman = 0;
}

int Fittingroom::get_status() {
    return status;
}

room_status Fittingroom::get_room_stts() {
    return rmst;
}

int Fittingroom::ready_to_leave(int i) {
    return users[i].ready_to_leave();
}

int Fittingroom::get_gender(int i) {
    return users[i].get_gender();
}