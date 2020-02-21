#Problem.3 Numberix
===================

**###1.Introduction** 


This program is for hw2 in Discrete Mathematics class (fall 2019).



**###2.Compile command** 

    chmod 755 build_script    
    ./build_script  



To execute the build script file, we need a permission. 
'chmod 755 build_script' command gives the permission.  
The input file have to named as 'input.txt'.
Also you can handle each steps for compiling.    

    gcc hw2.c  
    cat input.txt | ./a.out > Solution.txt      

Output is saved in Solution.txt



**###3. Input Example**

(or a1 (not (or (not (or a2 a3)) a4)))


**###4. Solution.txt**

Solution.txt file would look like this.

1 
2 -4 
3 -4 
0

solution
1 2 3 4 

'0' is delimeter.

if there is no solution it prints UNSAT

