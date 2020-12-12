Consider the linear model below:



$$ 2x_1 + x_2 \le 6 $$

$$ 2x_1 + 3x_2 \le 9 $$


max z=
<img src="https://render.githubusercontent.com/render/math?math=3x_1%2B4x_2 ">

<img src="https://render.githubusercontent.com/render/math?math=2x_1 %2Bx_2\le6 "> 

<img src="https://render.githubusercontent.com/render/math?math=2x_1%2B3x_2\le9">

<img src="https://render.githubusercontent.com/render/math?math=x_1 \ge 0, x_2\ge 0">

<img src="https://render.githubusercontent.com/render/math?math=e^{i %2B\pi} =x%2B1">


To solve the model, define the model to the program:

![max%203x1%204x2.png](attachment:max%203x1%204x2.png)

The optimal solution is:

<img src="https://render.githubusercontent.com/render/math?math=z=12.75, x_1=2.25, x_2=1.5">

The program output:

![max%203x1%204x2%20simplex.png](attachment:max%203x1%204x2%20simplex.png)

If we assume the variables are integer we can apply the branch and bound algorithm after obtaining the relaxed Linear solution 

The optimal integer solution will be:

<img src="https://render.githubusercontent.com/render/math?math=z=12, x_1=0, x_2=3">

Type 1 to get the integer optimal solutino. The output of the program is given below:

![max%203x1%204x2%20BB.png](attachment:max%203x1%204x2%20BB.png)
%2B
