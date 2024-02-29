/*
Decorator is a structural pattern that allows adding new behaviors to objects dynamically by placing them inside special wrapper objects, called decorators.

Using decorators you can wrap objects countless number of times since both target objects and decorators follow the same interface.
The resulting object will get a stacking behavior of all wrappers.
*/

package main

import "fmt"

type IPizza interface {
	getPrice() int
}

type VeggieMania struct {
}

type TomatoTopping struct {
	pizza IPizza
}

type CheeseTopping struct {
	pizza IPizza
}

func (p *VeggieMania) getPrice() int {
	return 15
}

func (c *TomatoTopping) getPrice() int {
	pizzaPrice := c.pizza.getPrice()
	return pizzaPrice + 7
}

func (c *CheeseTopping) getPrice() int {
	pizzaPrice := c.pizza.getPrice()
	return pizzaPrice + 10
}

func main() {

	pizza := &VeggieMania{}

	//Add cheese topping
	pizzaWithCheese := &CheeseTopping{
		pizza: pizza,
	}

	//Add tomato topping
	pizzaWithCheeseAndTomato := &TomatoTopping{
		pizza: pizzaWithCheese,
	}

	fmt.Printf("Price of veggeMania with tomato and cheese topping is %d\n", pizzaWithCheeseAndTomato.getPrice())
}

// Output: Price of veggeMania with tomato and cheese topping is 32
