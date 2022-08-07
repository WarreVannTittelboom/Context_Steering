# Context Steering
## Introduction
Context based steering behaviors are behaviors that aim to reduce complexity and improve effiency by eliminating the need for complex calculations to be constantly repeated, this is achieved with use of context maps.  <br/>
This also means that there is no need for loads of different steering behaviours, simplifying the code even further. <br/>
This is why context steering is considerd to be an improvement to regular steering behaviors. <br/>
## Context Maps/Steering Explained <br/>
A Context Map is essentially an array where each index represents a different direction. <br/>
If we would have a Context Map with size 8, each index would point to a cardinal or an ordinal direction. <br/>
If the size of the array is increased (the number of directions) the “resolution” of the map increases. <br/>
![Image_1](https://andrewfray.files.wordpress.com/2013/03/contextprojection.png?w=450&h=192)<br/>
Two instances of these maps are needed, one for Interest where the intrest value(quatitates likelihood of agent going this direction) <br/>
for each direction is stored and one for Danger where the danger value for each direction is stored if needed. <br/>
We can decide how the danger value is handled (if there is one), either we substract it from the corresponding intrest value or  <br/>
we ignore the corresponding intrest value in the final calculation. I have chosen the latter for my implementation. <br/>
By combining both values we can calculate a final direction of the agent. <br/>
### Simple Example
<img src="https://i.imgur.com/j731Bbo.png" width="600"> <br/>
The image above is a good example of the context steering behavior. <br/>
We can see that in the intreset array there is a high intrest value for the yellow direction and a low <br/>
intrest value for the blue direction this is because the target in the yellow direction is alot closer <br/>
then the target in the blue direction but because there is an obstacle in yellow direction we have a danger <br/>
value for this direction, as metioned before we will ignore the intrest value in any direction that has a danger value. <br/>
Thus, the agent will decide to go towards the lower intrest value direction instead.
## Implementation



## Conclusion


### Closing Thoughts

## References
https://andrewfray.wordpress.com/2013/03/26/context-behaviours-know-how-to-share/ <br/>
https://jameskeats.com/portfolio/contextbhvr.html <br/>
http://kidscancode.org/godot_recipes/ai/context_map/ <br/>

## Thank you for your time!

![Alt Text](https://c.tenor.com/WjoUFaID8ScAAAAC/cat-cute.gif)


