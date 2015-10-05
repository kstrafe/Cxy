# Complete Sample #

This file contains the complete sample. It describes a program that makes use of every
language facility. Each file is separated by a ===, with a filename structure appended.
The program is trying to be minimalistic.

=== Main.cxy

	(float out : float in : pure) sin  // A function
	{
		var float {  // var Type {} declaration
			twice = in * in;
			four = twice * twice;
			six = four * twice;
		};
		return in - twice*in/6 + four*in/120 - six*in/5040;
	}

	(float out : (float out : float in : pure) function : pure) ccdo
	{
		return function(0.312f);
	}

	(:) enter  // The main procedure
	{
		ccdo(lambda [](float out : float in){ return in - 100; });

		label start;  // Jump labels
		sml.Out << "sin(x), x = ";
		var sml.String value << sml.In;
		sml.Out.write("The computer value: ");
		var float result = sin(value.parseFloat());
		sml.Out
			<< result
			<< sml.String.line
			<< "Would you like to do another computation? (y/n): ";
		if (sml.In.read() == "y")  // If statement
			goto start;  // Goto jump
		else  // Else statement
			while (true)  // While looping
			{
				for (32u counter = 0; counter < 100; ++counter)  // For looping
					sml.Out << counter;
				break;
			}

		var sml.MersenneTwister64 generator(seed: sml.Time.getTime()~microseconds);
		var pac.Sword[Drbg: sml.Generator] sword(damage: 100, generator: $generator);
		var pac.Shield[Drbg: sml.Generator] shield(health: 100, generator: @generator);

		var 1u too_strong = isSwordTooStrong(sword: @@sword, shield: $$shield);
		var ptr const 1u constant = $$too_strong;
		sml.Out << ^constant;
		var ptr const ptr const 1u constant_2 = $$constant;
		sml.Out << ^^constant_2;

	}

	(1u out : cref pac.Sword sword, cptr pac.Shield shield : pure) isSwordTooStrong
	{
		return sword.damage > 100 * shield.health;
	}

=== pac/Sword.cxy

	grant Drbg has { (float out :: pure) getNext; }
	var 64u damage;
	var ptr Drbg random;

	(: this, 64u damage, ptr Drbg generator) Sword
	{
		this.damage = damage;
		this.random = generator;
	}

	(64u damage :: const pure) hit
	{
		return damage: cast[64u](random.getNext() * cast[float](damage));
	}

=== pac/Shield.cxy

	grant Drbg has { (float out :: pure) getNext; }

	var 64u health;

	(: this, 64 health) Shield
	{
		this.health = health;
		this.random = generator;
	}

	(64u taken : ref type[Drbg] generator, 64u damage : pure) takeHit
	{
		taken = cast[64u](random.getNext() * cast[float](damage));
		health -= taken;
	}

	static if (#compiler == "scxyc" && #version == "0.1.0-draft.2")
		var 64s adder;

	var sml.String some_string = #gen.Gen();


=== gen/Gen.cxy

	(String out :)
	{
		return out: """There are no words for it""";
	}

===

Current used:

* Entry point 'enter'
* @@ 'Constant Reference', @ 'Reference', $$ 'Pointer-To-Constant', $ 'Address-Of', ^^ 'Dereference twice', ^ 'Dereference once'
* []-Templating
* grants
* Named return parameters
* Named input parameters
* Single unnamed return
* Single unnamed input
* Type casting
* Free functions
* Pure functions
* const methods
* const pure methods
* ref/ptr declaration
* Compound operators
* type[] expression
* Class Instantiation
* Code Generation
* Static If
* Lambda Expression
