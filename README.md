Consider the linear model below:

$ \textrm{max z} = 3x_1 + 4x_2 $

$$ 2x_1 + x_2 \le 6 $$

$$ 2x_1 + 3x_2 \le 9 $$

$$ x_1 \ge 0, x_2\ge 0 $$

To solve the model, define the model to the program:

![max%203x1%204x2.png](attachment:max%203x1%204x2.png)

The optimal solution is:

$$ z=12.75, x_1=2.25, x_2=1.5 $$

The program output:

![max%203x1%204x2%20simplex.png](attachment:max%203x1%204x2%20simplex.png)

If we assume the variables are integer we can apply the branch and bound algorithm after obtaining the relaxed Linear solution 

The optimal integer solution will be:

$$ z=12, x_1=0, x_2=3 $$

Type 1 to get the integer optimal solutino. The output of the program is given below:

![max%203x1%204x2%20BB.png](attachment:max%203x1%204x2%20BB.png)
