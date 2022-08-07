# Context Steering
## Introduction
Context-based steering behaviors are behaviors that aim to reduce complexity and improve efficiency by eliminating the need for complex calculations to be constantly repeated(mainly when dealing with collision avoidance), this is achieved with use of context maps.  <br/>
This also means that there is no need for loads of different steering behaviors, simplifying the code even further. <br/>
This is why context steering is considered to be an improvement to regular steering behaviors. <br/>
## Context Maps/Steering Explained <br/>
A Context Map is essentially an array where each index represents a different direction. <br/>
If we would have a Context Map with size 8, each index would point to a cardinal or an ordinal direction. <br/>
If the size of the array is increased (the number of directions) the “resolution” of the map increases. <br/>
![Image_1](https://andrewfray.files.wordpress.com/2013/03/contextprojection.png?w=450&h=192)<br/>
Two instances of these maps are needed, one for Interest where the interest value(quantitates likelihood of agent going this direction) <br/>
for each direction is stored and one for Danger where the danger value for each direction is stored if needed. <br/>
We can decide how the danger value is handled (if there is one), either we subtract it from the corresponding interest value <br/>
using interpolation or we ignore the corresponding interest value in the final calculation. The latter results in obstacles being avoided <br/>
less smoothly compared to the former but the former may not be the result you want depending on use case. <br/>
By combining both values we can calculate the final direction of the agent. <br/>
### Simple Example
<img src="https://i.imgur.com/j731Bbo.png" width="600"> <br/>
The image above is a good example of the context steering behavior. <br/>
We can see that in the intrest array there is a high interest value for the yellow direction and a low <br/>
interest value for the blue direction this is because the target in the yellow direction is a lot closer <br/>
then the target in the blue direction but because there is an obstacle in yellow direction we have a danger <br/>
value for this direction, in this example we will substract danger value from the intrest value which <br/>
results in a value that is smaller then 0 because the obstacle is closer then the target, meaning we can ignore this value. <br/>
Thus, the agent will decide to go towards the lower interest value direction instead.
## Implementation
### Creating All Arrows
First off, we create a certain amount of arrows with a certain length. The more arrows we have, the fewer gaps there will be in between them <br/>
which will result in more accuracy but also more performance impact. We can also change the length to detect the obstacles sooner or later. <br/>
#### I decided to with 64 arrows. <br/>
<img src="https://i.imgur.com/Df1zJVj.png" width="400"> <br/>
### Determining the Intrest Values
Now we determine the values in our Intrest array (the first Context Map). Higher interest values mean the player <br/>
is more likely to go in that direction. Each value is in the range of 0 to 1, Now to actually determine these values <br/>
we will use the dot product on the arrow's direction vector and the direction vector from the agent to the target. <br/>
If the result of the dot product is below 0 we can say that the arrow is going in the opposite direction, thus <br/>
we set the interest value to 0. For the arrows that are not pointing in the opposite direction, we set the <br/>
interest value to the result of the dot product. <br/>
#### Applying the interest values to the arrows (interest * length)
<img src="https://i.imgur.com/QdzOFPW.png" width="400"> <br/>
Logically, this will result in half of the arrows being removed because they point in the <br/>
wrong direction(interest = 0) and also that the remaining arrow's length will be scaled to their interest.<br/>
### Determining the Danger Values
To account for all the obstacles we will make another array as we did before but this time we <br/>
will call it the Danger Array (the second Context Map) with the same 0 to 1 range as before. <br/>
Here we have to make decide how we will handle the danger values as mentioned before. <br/>
#### No Interpolation
If we decide to not use interpolation we simply set the danger value to 1 when the arrow is colliding <br/>
![Alt Text](https://i.imgur.com/yJWLJK8.gif)<br/>
#### Using Interpolation
If we decide to use interpolation we interpolate our danger value using the range of the obstacle. <br/>
This will result in a smoother but slightly different behavior.(as mentioned before) <br/>
![Alt Text](https://i.imgur.com/otJlEzl.gif)<br/>
Before we can determine our final direction we first have to subtract the danger values from our interest values. <br/>
So if we use the no interpolation implementation we either have a danger value of 0 or 1 meaning if the danger <br/>
value is 1 the corresponding direction will be completely ignored. But if we use the interpolation implementation <br/>
the interest will just be lowered by the corresponding danger value. <br/>
### Determining the Final Direction
We can now sum up all the arrow's directions multiplied by their final interest values (after subtracting danger values) <br/>
and divide them by the total amount of arrows. <br/>
#### That's it, now we have a simple but working Context Steering Behavior!


## Conclusion
Context-based steering is an easy-to-understand/implement solution when in need of a collision avoidance steering behavior. <br/>
If u would want to achieve a similar result with regular steering behaviors u would have to combine loads and use complex logic. <br/>
Context-based steering is mostly used for 2D games but can also be useful for the steering of AI cars in 3D racing games. <br/>

## References
https://andrewfray.wordpress.com/2013/03/26/context-behaviours-know-how-to-share/ <br/>
https://jameskeats.com/portfolio/contextbhvr.html <br/>
http://kidscancode.org/godot_recipes/ai/context_map/ <br/>

## Thank you for your time!

![Alt Text](https://c.tenor.com/WjoUFaID8ScAAAAC/cat-cute.gif)

