Suppose the faculty are going to have a party for the students in the department.
a. Identify tasks that can be assigned to the faculty members that will allow
them to use task-parallelism when they prepare for the party. Work out a
schedule that shows when the various tasks can be performed.
b. We might hope that one of the tasks in the preceding part is cleaning the
house where the party will be held. How can we use data-parallelism to
partition the work of cleaning the house among the faculty?
c. Use a combination of task- and data-parallelism to prepare for the party. (If
there’s too much work for the faculty, you can use TAs to pick up the slack.)

a. 
cleaning the house
set the room
prepare food

b.
5 people clean 5 parts of the department at the same time.

c.
5 people abcde
clean 5 parts in the department
then
ab prepare the food while cde set the rooms

        a       b       c       d       e
time1   clean1  clean2  clean3  clean4  clean5  
time2   food1   food2   set1    set2    set3
time3   food1   food2   set1    set2    set3
time4   food1   food2   set1    set2    set3