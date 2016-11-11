/*
 * AlgorithmInterface.h
 *
 *  Created on: 17 Feb, 2015
 *      Author: Colten Normore
 *
 * The Algorithm inteface is a template that all algorithms must implement to run
 * from main. In particular, an algorithm must implement int Pi2Main(), as it invoked
 * from main.
 */

#ifndef ALGORITHMINTERFACE_H_
#define ALGORITHMINTERFACE_H_

class AlgorithmInterface {
public:
	virtual int pi2Main() = 0;
};

#endif /* ALGORITHMINTERFACE_H_ */
