//-----------------------------------------------------------------------------------------------
// main.js
// (c) copyright Andreas Verhoeven, 2005.
//
// This file may be distributed only with Phil Caetano's AveScripter desklet for AveDesk.
// The code in this script may be used only for creating AveScripter desklets.
// Only with explicit permission this code may be re-used into something not AveScripter related.
//-----------------------------------------------------------------------------------------------
//
//
// Version 1.3:
//  + FIX trailing zero's are now showing up when entering a number like 0.00 [no eval() when 0.00 pattern]
//  - did not fix JScript rounding error
//
// Version 1.2:
// + FIX changed code so it would work on the newest AveScripter
//	     version [control.newIndex vs this.SelectCtrlIdex]
//
// Version 1.1
//	+ FIX no more leading zeros (eg. 00005) are showing up [using eval()]
//  + FIX fixed error of initializing the memory-cell to the empty-string instead of 0.
//  + FIX changed the operation of the memoryOps (M+, M-, MC, MR) to be consistent with
//		everyday calculators
//	+ ADDED precision
//
// Version 1.0
//	+ initial version
//  - no precision yet
//
//

// This small calculator script is modelled as a finite state machine (FSM).
// The FSM can be drawn as the follows:
//	
//       -num,eq-|          -num/op-|
//       |       \/         |       \/
//   ->(input_num1)--op--->(input_numb)
///        /\                    |   /\
//         num                   eq   |
//          \                    /    op
//           \                 |_     |
//            (show_expr_result)-------
//
//    

// variable keeping track of debug status
DBG = false;

// variables keeping track of the state
STATE_INPUT_NUM1	= 0;
STATE_INPUT_NUM2	= 1;
STATE_SHOWRES		= 2;
var state = STATE_INPUT_NUM1;

// variables used for input
var inputA = "0";
var inputB = "0";
var operator = "";
var memoryCell = "0";
MAX_LENGTH = 10;
var PERIOD = ".";

// some small debug functions

/**
 * Performs an 'error'. 
 * @return nothing
 */
function error()
{
	this.alert("Something went wrong in the calculator.");
}

/**
 * Returns the status of the stateInvariant.
 * @return [boolean], true iff the state-invariant holds.
 */
function stateInvariant()
{
	return STATE_INPUT_NUM1 == state || STATE_INPUT_NUM2 == state || STATE_SHOWRES == state;
}

/**
 * Function that simulates assert.
 * @param [boolean] b, if false and DBG is enabled/true, an error will be thrown/performed.
 * @return nothing.
 */
function assert(b)
{
	if(DBG)
	{
		if(!b)
			error();
	}
}


/**
 * Updates the precision for this desklet.
 * @return nothing.
 */
function updatePrecision()
{
	this.parameters("PRECISION").value = this.controls('droplistPrecision').newIndex;
	updateDisplay();
}

/**
 * Does a number-press input in the state-machine.
 * @param [char] theNumber - the number being pressed.
 * @return nothing.
 */
function numberPressed(theNumber)
{
	assert( stateInvariant() );

	// for each state of the FSM, do a (transition, action).
	if(STATE_INPUT_NUM1 == state)
	{
		// when inputting the first number, keep inputting it
		if(inputA.length < MAX_LENGTH)
			inputA += theNumber;

		// kill any leading zero's if there's no period in it
		if(inputA.indexOf(".") == -1)
			inputA = "" + eval(inputA);

		state = STATE_INPUT_NUM1;
	}
	else if(STATE_INPUT_NUM2 == state)
	{
		// when inputting the second number, keep inputting it
		if(inputB.length < MAX_LENGTH)
			inputB += theNumber;

		// kill any leading zero's if there's no period in it
		if(inputB.indexOf('.') == -1)
			inputB = "" + eval(inputB);

		state = STATE_INPUT_NUM2;
	}
	else if(STATE_SHOWRES == state)
	{
		// when showing the result, and a number is pressed
		// clear everything and start inputting the first number.
		inputA = "" + theNumber;
		inputB = "";
		operator = "";
		state = STATE_INPUT_NUM1;
	}
	else
	{
		error();
	}

	updateDisplay();

	assert( stateInvariant() );
}


/**
 * Tries to add a period to an input.
 * @param [string] input - the input to add a period to.
 * @return [string] the string with the period added, if possible.
 */
function addPeriodToInput(input)
{
	var retVal = input;
	// we only add a period if there's no period yet, because
	// two periods are not valid in a number.
	if(retVal.indexOf(PERIOD, 0) == -1)
	{
		if(retVal.length < MAX_LENGTH)
		{
			// use "0.decimal" instead of ".decimal"
			if(0 == retVal.length)
				retVal = "0";

			retVal += PERIOD;
		}
	}

	return retVal;
}


/**
 * Does a period input in the FSM.
 * @return nothing.
 *
 */
function addPeriod()
{
	assert( stateInvariant() );

	// for each state of the FSM, do a (transition, action).
	if(STATE_INPUT_NUM1 == state)
	{
		// when inputting the first number, only add a period if there is
		// no period present already
		inputA = addPeriodToInput(inputA);

		state = STATE_INPUT_NUM1;
	}
	else if(STATE_INPUT_NUM2 == state)
	{
		// when inputting the first number, only add a period if there is
		// no period present already
		inputB = addPeriodToInput(inputB);

		state = STATE_INPUT_NUM2;
	}
	else if(STATE_SHOWRES == state)
	{
		// when showing the result, and a period is pressed
		// clear everything and start inputting the first number as a decimal.
		inputA = "0" + PERIOD;
		inputB = "";
		operator = "";
		state = STATE_INPUT_NUM1;
	}
	else
	{
		error();
	}

	updateDisplay();

	assert( stateInvariant() );
}

/**
 * Does a memory-operation input for the FSM.
 * @param [string] theOp - the memory-operation to do.
 * @return nothing.
 */
function memoryOperation(theOp)
{
	assert( stateInvariant() );

	theMemOp = theOp.substr(1,1);

	// because each memOp always goes to the same state from every state [except for mr],
	// there is no need to select actions on states, but to select
	// actions on the memory-operation.
	if("r" == theMemOp)
	{
		if(STATE_INPUT_NUM1 == state || STATE_SHOWRES == state)
		{
			// recall the memory and go into input state 1
			inputA = memoryCell;
			inputB = "";
			operator = "";
			state = STATE_INPUT_NUM1;
		}
		else if(STATE_INPUT_NUM2 == state)
		{
			inputB = memoryCell;
			state = STATE_INPUT_NUM2;
		}
		else
		{
			error();
		}
	}
	else if("c" == theMemOp)
	{
		// clear the memorycell, no state transition, because
		// this input always has a transition from the state 
		// back to the same state.
		memoryCell = "0";
	}
	else
	{
		// if we have a plus or minus mem-op, do it!
		var tempRes = "0";
		if(STATE_INPUT_NUM1 == state)
			tempRes = inputA;
		else if(STATE_INPUT_NUM2 == state)
			tempRes = getResult();
		else if(STATE_SHOWRES == state)
			tempRes = getResult();

		// make sure we have a legal expression in eval()
		tempRes = (tempRes.length > 0 ? tempRes : "0");

		// calculate the new contents of the memory cell
		memoryCell = "" + eval(memoryCell + theMemOp + tempRes);
	}

	updateDisplay();

	assert( stateInvariant() );
}

/**
 * Does an operator input in the FSM.
 * @param [char] theOp - the operator being pressed.
 * @return nothing.
 */
function operation(theOp)
{
	assert( stateInvariant() );

	// for each state, do something.
	if(STATE_INPUT_NUM1 == state)
	{
		var inputAIsZero =  inputA.indexOf('.') == -1 && (0 == "" + eval(inputA));
		if("-" == theOp && (0 == inputA.length || inputAIsZero))
		{
			inputA = "-";
			state = STATE_INPUT_NUM1;
			updateDisplay();
		}
		else
		{
			// if the first number was being inputted,
			// remember the operator and start inputting the second number.
			operator = theOp;
			state = STATE_INPUT_NUM2;
		}
	}
	else if(STATE_INPUT_NUM2 == state || STATE_SHOWRES == state)
	{
		// if the second number was being inputted,
		// calculate the result of the the previous sequence,
		// and assign it to the first number. Then, start 
		// inputting the second number.

		state = STATE_INPUT_NUM2;

		var theTempRes = getResult();
		inputA = theTempRes;
		inputB = "";
		operator = theOp;

		updateDisplay();
	}
	else
	{
		error();
	}

	assert( stateInvariant() );
}


/**
 * Gets the result of the inputs and the operator.
 * @return [string] the result of inputA 'operator' inputB.
 */
function getResult()
{
	assert( stateInvariant() );

	var a = (inputA.length > 0 ? inputA : "0");
	var b = (inputB.length > 0 ? inputB : "0");

	return "" + eval(a + operator + b);
}

/**
 * Does an evaluate input in the FSM.
 * @return nothing.
 */
function evaluate()
{
	assert( stateInvariant() );

	// for each state, define what we do.
	if(STATE_INPUT_NUM1 == state)
	{
		// when inputting the first number, an [=] does nothing.
		state = STATE_INPUT_NUM1;
	}
	else if(STATE_INPUT_NUM2 == state)
	{
		// when inputtign the second number, an [=]
		// makes the expression complete, thus we show its 
		// result.
		state = STATE_SHOWRES;
		updateDisplay();
	}
	else if(STATE_SHOWRES == state)
	{
		// repeat the previous operator:inputB when doing an [=] on a result.
		inputA = getResult();
		state = STATE_SHOWRES;
		updateDisplay();
	}
	else
	{
		error();
	}

	assert( stateInvariant() );
}

/**
 * Does a clear input in the FSM.
 * @return nothing.
 */
function clearInput()
{
	assert( stateInvariant() );

	// this transition does the same for every state.
	if(STATE_INPUT_NUM1 == state || STATE_INPUT_NUM2 == state || STATE_SHOWRES == state)
	{
		// go back to the starting state, with the same data as the calculator started.
		inputA = "0";
		operator = "";
		inputB = "";
		state = STATE_INPUT_NUM1;
	}
	else
	{
		error();
	}

	updateDisplay();

	assert( stateInvariant() );
}

/**
 * Updates the display of the calculator.
 * @return nothing.
 */
function updateDisplay()
{
	assert( stateInvariant() );

	var text = "0";

	// for each state, we show a different text in the calculators display.
	if(STATE_INPUT_NUM1 == state)
	{
		// when inputting the first number, show the first number - if actually something 
		// was inputted.
		if(inputA.length > 0)
			text = inputA;
	}
	else if(STATE_INPUT_NUM2 == state)
	{
		// when inputting the second number, show the second number if actually
		// something was inputted for the second number. Otherwise, show the first
		// number.
		if(inputB.length > 0)
			text = inputB;
		else
			text = inputA;
	}
	else if(STATE_SHOWRES == state)
	{
		// when showing the result, .... show the result :-).
		text = getResult();
	}

	var numVal = "" + text;
	if(text.indexOf(".") == -1)
		numVal = eval(text);

	var precision = this.parameters("PRECISION").value;
	if(precision > 0)
		numVal = numVal.toFixed(precision);

	this.layers('display').text = "" + numVal;

	assert( stateInvariant() );
}