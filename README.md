# Zipper Iterator

**Author**: Alfredo A. Correa

**Contact**: correaa@llnl.gov

**Organization**: Lawerence Livermore National Laboratory

**Date**: Feb 9 2017

**Copyright**: Alfredo A. Correa 2017

**Abstract**: 
The `zipper` iterator provides the abilitiy to parallel-iterate over several controlled sequences simultanously.
A `zipper` iterator is constructed from other iterators.
The category of semantics the `zipper` iterator dependes on the underlying iterator from which `zipper` iterator is contructed from.
In the most general case, it and input iterator that is readable, writable and swappable.
Moving the `zipper` iterator moves all the underlying iterators in parallel. 
Dereferencing the `zipper` iterator return a tuple of references from the underlying iterators.

(Not an official Boost component at the moment)

## `zipper` synopsis

    template<class... Iterators>
    class zipper{
      using difference_type = /*common difference type*/
      using value_type = /*associated tuple of values*/
      using pointer = zipper;
      using reference = /*associated tuple of references*/
      using iterator_category = /*common iterator category*/
      zipper();
      zipper(Iterators... its);
      reference operator*() const;
      zipper& operator-=; // and logically associated operators
      zipper& operator+=; // and logically associated operators
      zipper& operator++(); // and logically assoc. ops.
      friend bool operator<(zipper const&, zipper const&);
      friend void iter_swap(zipper const&, zipper const&);
    }

    template <class... Iterators> 
    zipper<Iterators...> make_zipper(Iterators... vi);}
    
    template <class... Iterators>
    zipper<Iterators...> zip(Iterator&&... vi)

There reference member of `zipper` is the type of the tuple made of the reference types.

The difference type member of `zipper` is the common type of the underlying difference types.

## `zipper` requirements

All iterators need to be at least input iterators.

## `zipper` models

The resulting `zipper` iterator models an Readable Writable Swappable Iterator.

`zipper` iterator interoperates with `sort` algorithm if all the underlying iterators are random access.

## `zipper` operations

`zip_iterator();`

**Returns**: (constructor) an instance of `zipper` with undelaying default constructed iterators.

`zip_iterator(Iterators... its);`

**Returns**: (constructor) an instance of `zipper` with undelaying iterators `its`.

`reference operator*() const;`

**Returns**: A tuple of references

`zipper& operator==();` 

`zipper& operator++();` 

`zipper& operator--();` 

`zipper& operator+=();` 

`zipper& operator-=();` 


and all associated algebraic operations.
Each can be used if all the underlying iterators support the specific operation.

`template<Iterator...> make_zipper(Iterators...);` 

**Returns**: A `zipper` of iterators (copies are made).

`template<Iterator...> zip(Iterators&&...)` 

**Returns**: A `zipper` of iterators or references to iterators if the context allows.


## Examples

Sort is a general enough algorithm to show the power of the `zipper` iterator.

		std::vector<double> vv;
		std::vector<double> ww;
		std::sort(
			zip(begin(vv), begin(ww)),
			zip(end(vv)  , end(ww)  ),
			[](auto&& a, auto&& b){return get<1>(a) < get<1>(b);}
		);

(with minimal compiler optimizations it operates as if manually).

## Note on ranges

Ranges of `zipper` iterators are valid only if all the underlying iterators are compatible ranges.
Validity of ranges is not checked.

## Note on the standard iteroperability

For `zipper` to work transparently (and efficiently) with STL all algorithms I had to specialize `std::iter_swap`, which is currently in violation of the specification.

## Note on construction semantics

The class doesn't use the `std::ref` convention, references are handled by `make_zipper` and `zip` functions. 
The first makes copies of all the underlying interators and the second captures references to the iterators when possible.

For example, `z1` and `z2` are the same type (`zipper<vector<double>::iterator, vector<double>::iterator>`)
 
	auto z1 = zip(v.begin(), w.begin());
	auto z2 = make_zipper(v.begin(), w.begin());

but not `z3` (`zipper<vector<double>::iterator&, vector<double>::iterator&>`)

	auto b1 = v.begin();
	auto b2 = v.begin();
	auto z3 = zip(b1, b2);

(in this case `b1` and `b2` can get modified trough modifications of `z3`, and dangling references could occur if `b1`/`b2` went out of scope).

## Related work

http://www.boost.org/doc/libs/1_63_0/libs/iterator/doc/zip_iterator.html
(limited Readable semantics, does not interoperate with `sort` and other mutating algorithms.)
