Consider the linear model below:

max z=
<img src="https://render.githubusercontent.com/render/math?math=z=3x_1%2B4x_2 ">

<img src="https://render.githubusercontent.com/render/math?math=2x_1 %2Bx_2\le6 "> 

<img src="https://render.githubusercontent.com/render/math?math=2x_1%2B3x_2\le9">

<img src="https://render.githubusercontent.com/render/math?math=x_1 \ge 0, x_2\ge 0">

<img src="https://render.githubusercontent.com/render/math?math=e^{i %2B\pi} =x%2B1">


To solve the model, define the model to the program:

![max 3x1 4x2](https://user-images.githubusercontent.com/57063453/101983952-45903280-3c8f-11eb-96a5-18d204037382.png)


The optimal solution is:

<img src="https://render.githubusercontent.com/render/math?math=z=12.75, x_1=2.25, x_2=1.5">

The program output:

![max 3x1 4x2 simplex](https://user-images.githubusercontent.com/57063453/101983982-7a9c8500-3c8f-11eb-9a76-043d2260117e.png)

If we assume the variables are integer we can apply the branch and bound algorithm after obtaining the relaxed Linear solution 

The optimal integer solution will be:

<img src="https://render.githubusercontent.com/render/math?math=z=12, x_1=0, x_2=3">

Type 1 to get the integer optimal solutino. The output of the program is given below:

![max 3x1 4x2 BB](https://user-images.githubusercontent.com/57063453/101983997-93a53600-3c8f-11eb-89a6-621de869d519.png)
