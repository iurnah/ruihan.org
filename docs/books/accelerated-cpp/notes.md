# Accelerated C++ Reading Notes

## Chapter 0

1. Expression -> results + side effects
2. Every operand has a type `std::out` has type `std::ostream`
3. `<<` is left-associative, mean: `(std::cout << "Hello, world!") << std::endl`
4. `int main(){}` return 0 if return sccessful, otherwise return failure.
5. exercise 0-7 tell us comments like this wouldn't work `/*comments1 /*comment2*/comment3*/` is illigle.
6. exercise 0-8 tell us comments like this would work fine

    ```C++
    // This is a comment that extends over several lines
    // by using // at the beginning of each line instead of using /*
    // or */ to delimit comments.
    ```

## Chapter 1

1. `>>`  begins by discarding whitespace chars (space, tab, backspace, or the
   end of line) from the input, then reads chars into a virable until it
   encounters another whitespace character or end-of-file.
2. input-output library saves its output in an internal data structure called a
   buffer, which it uses to optimize output operations. it use the buffer to
   accumulate the characters to be written, and flushes the buffer, only when
   necessary.
3. 3 events cause the flush:
    1. when it is full;
    2. when it is asked to read from the std input;
    3. when we explicitly say to do so;
4. How to print a framed string?
5. std::string constructor: `const std::string greeting(greeting.size(), ' ')`
   will generate a whitespace string with length of greeting.size().
6. character literal should be enclosed by `' '`, string literal should be
   enclosed by `" "`.
7. Built-in type `char` and `wchar_t`, which is big enough for holding characters
   for languages such as Chinese.

## Chapter 2

1. `while` statement, __loop invariant__: which is a property that we assert
   will be true about a while each time it is about to test its condition.
   i.e. _write r line so far_

    ```C++
    // invariant: we have written r rows so far
    int r = 0;
    // setting r to 0 makes the invariant true
    while (r != rows) {
        // we can assume that the invariant is true here
        // writing a row of output makes the invariant false
        std::cout << std::endl;
        // incrementing r makes the invariant true again
        ++r;
    }
    // we can conclude that the invariant is true here The
    ```

2. `std::string::size_type` type for strings length or . This is to protect the
   int from being overflow if we have an arbitrary long input.
3. Asymmetric range [n, m) is better then a symmetric range [n, m]. always start
   the range with 0 such as [0, n)
    1. `[n, m)` have `m-n` elements, `[n, m]` have `m-n+1` elements.
    2. empty range i.e. `[n,n)`.
4. modulo operation equivalent : `x % y <==> x - ((x/y) * y)`
5. pay attention to the problem 2-4, adding code to ensure the invariant for
   outer while.

## Chapter 3

1. double is even faster than float, double has at least 15 significant digits.
   float  has at least 6 digits.
2. `while(cin >> x)` is equal to `cin >> x; while(cin)`, because `>>` operator
   return its left operand. `cin` is `istream` type, using `cin` as a condition
   is equivalent to testing whether the last attempt to read from `cin` was successful.
3. unsuccessful read by `cin`:
   1. We might have reached the end of the input file.
   2. We might have encountered input that is incompatible with the type of the
      variable that we are trying to read, such as might happen if we try to
      read an `int` and find something that isn't a number.
   3. The system might have detected a hardware failure on the input device.
4. save previous precision (for later reset) and set precision to 4:
   `streamsize prec = cout.pecision(4);`. precision manipulator `setprecision(3)`
   which means keep 3 significant digits (i.e. 56.5 or 5.65).
5. `std::vector<double>::size_type` is analogous to the one in `string::size_type`,
   its the type used to declare size type variable such as array size or string
   size. It usually `typedef std::size_t string::size_type`. It is guaranteed to
   be able to hold the number of elements in the largest possible `vector` or
   `string`.
6. sort function in `<algorithm>` header prototype:
  `sort(homework.begin(), homework.end())`, __sort in place__
7. vector class provide two member functions: `begin()` and `end()`.
8. calculate the median of a sorted vector homework:

    ```C++
    vec_sz mid = size/2;
    double median;
    median = size % 2 == 0 ? (homework[mid] + homework[mid-1]) / 2 : homework[mid];
    ```

9. `vec.end()` return a value that denotes one past the last elelment in v.
10. `streamsize` The type of the value expected by `setprecision()` and returned
   by `precision()`. Defined in  `<ios>`.
11. The return value of the `vector<int>::type_size` is unsigned integral number.
   Doing operation with it could not possibly generate negative value, i.e. if
   `vec.size()` is 5, `vec.size() - 6` is not negative, it will be positive.

## Chapter 4 (organize program `.h` and `.cc` files, iostream as a argument, exception handling basics)

1. When a program throws an exception, the program stop at the part of the
   program in which the `throw` appears. appears, and passes to another part of
   the program, along with an __exception object__, which contains information
   that the caller can use to act on the exception.
2. Domain error: `throw domain_error("median of an empty vector")`; Defined in
   `<stdexcept>`, it is used in reporting that a function's argument is outside
   the set of values that the function can accept.
    1. domain error is one of `logic_error` in `<stdexcept>`, there are other
       type of exception: `runtime_error`, which includes `overflow_error`,
       `underflow_error`, etc. [`runtime_error`](http://www.cplusplus.com/reference/stdexcept/runtime_error/).
3. `&` means a reference or an alias. `const` when define a reference means we
   will promise not modify the variable. For example,

    ```C++
    vector<doubel> homework;
    const vector<doubel>& chw = homework; //chw is a READ ONLY synonym for homework
    ```

4. A reference to reference is the same as reference to the original variable.
5. when define non `const` reference, we have to make sure the original variable
   or reference isn't declared `const`. Otherwise, it will be illigel. i.e. with
   above definition, we cannot do this `vector<doubel>& hw2=chw;` because `chw`
   is `const` when defined.
6. a `const` argument could take a const parameter.
7. `iostream` as a parameter to a function: (alwasy keep in mind that a iostream
   is a __type__, it has other properties such as a `vector` or `int` have.).
   Notice the returned value is `in`, which is passed in as a reference parameter.

    ```c++
    istream& read_hw(istream& in, vector<double>& hw) {

        // statement that modify the input parameters

        return in;
    }
    // the return of istream allow us to do the following
    if (read_hw(in, homework)) { }
    // othewise, we have to
    read_hw(in, homework);
    if (in) {

    }
    ```

8. The __"lvalue"__ We must pass an lvalue argument to a reference parameter. An
   "lvalue" is a value that denotes a __nontemporary__ object. For example, a
   variable is an `lvalue`, as is a reference, or the result of calling a function
   that returns a reference. An expression that generates an arithmetic value,
   such as `sum / count`, is __not__ an lvalue.
9. member functions `istream.clear()`. This is to ensure the eof or non-valid
   input data will not effect reading the next data. __Alwasy run `cin.clear()`,
   before we try to read again__.
10. pass by value: `vector<double> vec`: this will copy the argument, the
   original will not be modified.
11. pass by reference: `vector<double>& vec`: this will __not__ copy the argument,
   but will modify the original argument. This is good convention for object as
   a parameter, because copy a object have overhead.
12. pass by const reference: `const vector<double>& vec`: this will __not__ copy
   the argument as well as _promise_ not to modify the passed argument.
13. `try {} catch {}` clause, we normally break down to multiple statements in
   the `try` clause, because we want to avoid multiple side effect. For example,

    ```C++
    try {
            double final_grade = grade(midterm, final, homework);
            streamsize prec = cout.precision();
            cout << "Your final grade is " << setprecision(3)
                << final_grade << setprecision(prec) << endl;
    } catch (domain_error) {
            cout << endl << "You must enter your grades. "
            "Please try again." << endl;
            return 1;
    }
    ```

    is better then written as

    ```C++
    try {
            streamsize prec = cout.precision();
            cout << "Your final grade is " << setprecision(3)
                 << grade(midterm, final, homework) << setprecision(prec);
    }
    ```

    because the later can generate ambigious error message that not easy to debug.

14. sort object `Student_info`, we have to use another form of
   `sort(student.begin(), students.end())` with a extra parameter, which is a
   predicate to compare the two object. i.e.

    ```c++
    bool compare(const Student_info& x, cosnt Student_info& y)
    {
        return x.name < y.name;
    }
    sort(students.begin(),students.end(), compare);
    ```

15. Formatting the output, if we want to do the following

    ```text
    Bob         88
    Christopher 90
    ```

    we could do this:

    ```c++
    maxlen = max(maxlen, record.name.size());
    cout << students[i].name << string(maxlen + 1 - students[i].name.size(), ' ');
    ```

    Notice the `string(num, ' ')` instantiate a string has `num` of spaces.

16. hearder file should declare only the names that are necessary. Header files
   should use fully qualified names rather than using-declarations. (Avoid
   `using namespace std`;)

    ```c++
    #include <vector>
    double median(std::vector<double>);
    ```

17. Avoid multiple inclusion

    ```C++
    #ifndef __THIS_HEADER_H__
    #define __THIS_HEADER_H__
    //your program
    #endif
    ```

18. type of exceptions

    |   logic_error   |  domain_error  |
    |-----------------|----------------|
    |length_error     | out_of_range   |
    |invalid_argument | runtime_error  |
    |range_error      | overflow_error |
    |underflow_error  | |

19. exceptional handling

    ```C++
    try {
            // code Initiates a block that might throw an exception.
    } catch(t) {
            // code
    }
    //real use case.
    try {
            double final_grade = grade(students[i]);
            streamsize prec = cout.precision();
            cout << setprecision(3) << final_grade
                << setprecision(prec);
    } catch (domain_error e) {
            cout << e.what();
    }
    ```

    Concludes the `try` block and handles exceptions that match the type `t`.
    The code following the `catch` performs whatever action is appropriate to
    handle the exception reported in `t`.

20. `throw e`; Terminates the current function; throws the value `e` back to the caller.
21. `e.what()`: return a value that report on what happened to cause the error.
22. `str.width([n])` and `std::setw(n)` both used to set the output width.
23. The example program from this chapter is worth of keeping here for
   references. There is a lot information included in it.

```C++
//Calculate the grade for many students, including reading the data in, 
//how to sort according there name and how to format the out put in a nice printing, ect.
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::domain_error;
using std::endl;
using std::istream;
using std::ostream;
using std::setprecision;
using std::sort;
using std::streamsize;
using std::string;
using std::vector;

// compute the median of a `vector<double>'
// note that calling this function copies the entire argument `vector'
double median(vector<double> vec) {
#ifdef _MSC_VER
    typedef std::vector<double>::size_type vec_sz;
#else
    typedef vector<double>::size_type vec_sz;
#endif

    vec_sz size = vec.size();
    if (size == 0)
    throw domain_error("median of an empty vector");

    sort(vec.begin(), vec.end());

    vec_sz mid = size/2;

    return size % 2 == 0 ? (vec[mid] + vec[mid-1]) / 2 : vec[mid];
}

// compute a student's overall grade from midterm and final exam grades and homework grade
double grade(double midterm, double final, double homework) {
    return 0.2 * midterm + 0.4 * final + 0.4 * homework;
}

// compute a student's overall grade from midterm and final exam grades
// and vector of homework grades.
// this function does not copy its argument, because `median' does so for us.
double grade(double midterm, double final, const vector<double>& hw) {
    if (hw.size() == 0)
    throw domain_error("student has done no homework");

    return grade(midterm, final, median(hw));
}

// read homework grades from an input stream into a `vector<double>'
istream& read_hw(istream& in, vector<double>& hw) {
    if (in) {
    // get rid of previous contents
    hw.clear();

    // read homework grades
    double x;
    while (in >> x)
        hw.push_back(x);

    // clear the stream so that input will work for the next student
    in.clear();
    }

    return in;
}

int main() {
    // ask for and read the student's name
    cout << "Please enter your first name: ";
    string name;
    cin >> name;
    cout << "Hello, " << name << "!" << endl;

    // ask for and read the midterm and final grades
    cout << "Please enter your midterm and final exam grades: ";
    double midterm, final;
    cin >> midterm >> final;

    // ask for the homework grades
    cout << "Enter all your homework grades, "
    "followed by end-of-file: ";

    vector<double> homework;

    // read the homework grades
    read_hw(cin, homework);

    // compute and generate the final grade, if possible
    try {
    double final_grade = grade(midterm, final, homework);
    streamsize prec = cout.precision();
    cout << "Your final grade is " << setprecision(3)
        << final_grade << setprecision(prec) << endl;
    } catch (domain_error) {
    cout << endl << "You must enter your grades.  "
        "Please try again." << endl;
    return 1;
    }

    return 0;
}
```

## Chapter 5 (sequential containers (vector, list) and analyzing strings)

### 5.1

write a function `extract_fails()` to seperate the students that failed the course.
The ideas is to use two seperate vector to hold the ones that passed and the
ones that failed. To make it better, we only use one vector `fails` to hold the
failed students, and erase them from the original vector. however, be cautions
in using the `erase` memeber function for vectors. 1. the indexes of element
after the removed element will change. 2. the size will change. 3. efficiency
problem.

Introduced the necessity of using `iterator`. Briefly, it is for efficiency
optimization. The indexing is random access, which is more expensive to maintain
the data structure properties, such as for vector. `iterator` allow us to
separate the data access manner (sequential v.s. random) in a container, so as
to implement different container to cope with a different need. All this work is
because of efficiency concerns.

### 5.2

Beside providing access or modify operations, a iterator is able to restrict the
available operations in ways that correspond to what the iterator can handle
efficiently. Generally, two type of iterators: `const_iterator` and `iterator`.
when we do `vector<student>::const_iterator iter = S.begin()`, there is an
explicit type conversion happened because `S.begin()` is a type of `iterator`.

The usage of iterator `vector<int>::iterator iter;`, either `iter->name`
or `(*iter).name`.

### 5.5

vector and list differ in that if you call `erase(iter)` member function of the
container. vector will invalidate all the iterators following iter in the
vector. Even this is the case, we can use `erase` to delete a element from the
container. i.e. `iter = students.erase(iter);`, `iter` will point to the next
element of the removed element.

Similarly, call `push_back` member function will invalidate all the
iters of the vector. while for list container, call `erase(iter)` only
invalidate the `iter` erased, not others. and call `push_back` will not
invalidate other iterators.

Because list doesn't support random access. We cannot use the `sort()` function from
`<algorithm>`. Instead we have to use the member function that optimized for list
container.

### 5.8

Use `inset` function to do vertical concatenation ```ret.insert(ret.end(),
bottom.begin(), bottom.end())```

### Notes

1. `students.erase(students.begin()+i)`, remove the ith object in the container `students`.
2. `iterator` properties:
    3. Identifies a container and an element in the container
    4. Lets us examine the value stored in that element
    5. Provides operations for moving between elements in the container
    6. Restricts the available operations in ways that correspond to what the container can handle efficiently

3. `iterator` types:
    4. `container-type::iterator`: to change the container value
    5. `container-type::const_iterator`: to only read the container value
    6. `container-type::reverse_iterator`
    7. `container-type::const_reverse_iterator`
4. `*iter` return a lvalue. we can replace `(*iter).name` with `iter->name`
5. `students.erase(iter)` will invalidate all the iterators following the elements that has been removed.
    After `iter = students.erase(iter)`, `iter` will point to the first element following the removed element.
6. From `vector` to `list`.

    | vector                             | list                                    |
    |------------------------------------|-----------------------------------------|
    |optimized for fast __random access__|optimized for fast insertion and deletion|
    |Using `push_back` to append an element to a vector __invalidates__ all iterators referring to that vector.|the `erase` and `push_back` operations do not invalidate iterators to other elements.|

7. `list`: doesn't support random access, so the STL `<algorithm>` library function `sort()` doens't apply to `list`
8. `string` example, split a sentence into words.
```C++
vector<string> split(const string& s) {
    vector<string> ret;
    typedef string::size_type string_size; string_size i = 0;
    // invariant: we have processed characters [original value of i, i)
    while (i != s.size()) {
        // ignore leading blanks
        // invariant: characters in range [original i, current i) are all spaces
        while (i != s.size() && isspace(s[i]))
            ++i;

        // find end of next word string_size
        string_size j = i;
       // invariant: none of the characters in range [original j, current j)is a space
        while (j != s.size() && !isspace(s[j]))
            j++;
        // if we found some nonwhitespace characters
        if (i != j){
            // copy from s starting at i and taking j - i chars
            ret.push_back(s.substr(i, j - i));
                i = j;
        }
    }
    return ret;
}
```
9. `isspace` is in the header file `<cctype>`
    10. `isspace(c)` true if c is a whitespace character.
    11. `isalpha(c)` true if c is an alphabetic character.
    12. `isdigit(c)` true if c is a digit character.
    13. `isalnum(c)` true if c is a letter or a digit.
    14. `ispunct(c)` true if c is a punctuation character.
    15. `isupper(c)` true if c is an uppercase letter.
    16. `islower(c)` true if c is a lowercase letter.
    17. `toupper(c)` Yields the uppercase equivalent to c
    18. `tolower(c)` Yields the lowercase equivalent to c
10. In the `ret.push_back(s.str(i, j-i))`, the `j-i` indicate a open range `[s[i], s[j])`
11. `while (cin >> s)` is read one work at a time, because the `std::cin` seperated by white spaces. It terminate until a invalid input is entered or a EOF.
12. `while (getline(cin, s))` is reading one line at a time, it return false when EOF entered or invalid chars.
13. How to framing a word characters.
14. How to cancatenate two vector?,
    15. we can do `insert(ret.end(), bottom.begin(), bottom.end())`, note the first argument provide the iterator __before which__ the element will be inserted.
15. How to concatenate two pictures horizontally like the bellow
    ```
    pictures in case 1:
    this is an	 **************
    example		* this is an * 
    to			 * example    *
    illustrate	 * to		 *
    framing  	  * illustrate * 
                   * framing    *
                   **************
    pictures in case 2:
    ************** this is an
    * this is an * example
    * example    * to
    * to		 * illustrate
    * illustrate * framing
    * framing    * 
    **************
    ```
```c++
vector<string> hcat(const vector<string>& left, const vector<string>& right) {
    vector<string> ret;
    // add 1 to leave a space between pictures
    string::size_type width1 = width(left) + 1;
    // indices to look at elements from left and right respectively
    vector<string>::size_type i = 0, j = 0;
    // continue until we've seen all rows from both pictures
    while (i != left.size() || j != right.size()) {
        // construct new string to hold characters from both pictures
        string s;
        // copy a row from the left-hand side, if there is one
        if (i != left.size())
            s = left[i++];

        // pad to full width
        s += string(width1 - s.size(), ' ');

        // copy a row from the right-hand side, if there is one
        if (j != right.size())
            s += right[j++];

        // add s to the picture we're creating
        ret.push_back(s);
    }
    return ret;
}
```
16. `vec.reserve(n)`: Reserves space to hold n elements, but does not initialize them. This operation does not change the size of the container. It affects only the frequency with which vector may have to allocate memory in response to repeated calls to insert or push_back.
17. `c.rbegin()` and `c.rend()` are iterator refering to the last and (one beyond) the first element in the container that grant access to the container's elements in reverse order.

## Chapter 6

## Chapter 7

## Chapter 8

1. The language feature that implements generic functions is called template functions.
2. template header `template<class T>`
3. __"instantiation"__
4. Keyword `typename`, i.e. `typedef typename vector<T>::size_type vec_sz;` "you must precede the entire name by `typename` to let the implementation know to treat the name as a type."
5. The C++ standard says nothing about how implementations should manage
   template instantiation, so every implementation handles instantiation in its
   own particular way. While we cannot say exactly how your compiler will handle
   instantiation, there are two important points to keep in mind: The first is
   that for C++ implementations that follow the traditional edit-compile-link
   model, instantiation often happens not at compile time, but at link time. It
   is not until the templates are instantiated that the implementation can
   verify that the template code can be used with the types that were specified.
   Hence, it is possible to get what seem like compile-time errors at link time.
   The second point matters if you write your own templates: Most current
   implementations require that in order to instantiate a template, the
   definition of the template, not just the declaration, has to be accessible to
   the implementation. Generally, this requirement implies access to the source
   files that define the template, as well as the header file. How the
   implementation locates the source file differs from one implementation to
   another. Many implementations expect the header file for the template to
   include the source file, either directly or via a #include. The most certain
   way to know what your implementation expects is to check its documentation.

6. parameter type to a generic function should keep consistent. For example, We cannot pass int and double to the following function:
```C++
tmeplate <class T>
T max(const T& left, const T& right)
{
    return left > right ? left : right;
}
```
7. Data structure indepnedence: why we write the find function as `find(c.begin(), c.end(), val)`? (it is the only way to write generic functions that works on more than 1 element types)
8. Why not write as the form `c.find(val)` or `find(c, val)`?
9. iterator categories:

    1.Sequential read-only access (input iterator)
    2.Sequential write-only access (output iterator)
    3.Sequentila read-wirte access (input-output iterator)
    4.Reverseible access
    5.Random access

10. "input iterator" - interator support "++, ==, !=, unary *, and it->first". We say we give `find` two input iterators as parameters.
```C++
template<class In, class X>
In find(In begin, In end, constX& x)
{
    if(begin == end || *begin == x)
        return;

    begin++;
    return find(begin, end, x);
}
```
11. "output iterator" -interator support `*dest = _value_, dest++, and ++dest`
```C++
template<class In, class Out>
Out copy(In begin, In end, Out dest)
{
    while (begin != end)
        *dest++ = *begin++;

    return dest;
}
```
12. "input-output iterator" - iterator support `*it, ++it, it++, (but not --it or it--), it == j, it != j, it->member`
```C++
template<class For, class X>
void replace(For beg, For end, const X& x, const X& y)
{
    while (beg != end){
        if (*beg == x)
            *beg = y;

            ++beg;
    }
}
```
13. "reverse interator" - also support `--it` and `it--`
```C++
template<class Bi> void reverse(Bi begin, Bi end)
{
    while (begin != end) {
        --end;
        if (begin != end)
            swap(*begin++, *end);
    }
}
```
14. Random access - support `p + n, p - n, n + p, p-q, p[n], (equivalent to *(p + n)) p < q, p > q, p <= q, and p >= q `
```C++
template<class Ran, class X>
bool binary_search(Ran begin, Ran end, const X& x)
{
    while (begin < end) {
        // find the midpoint of the range
        Ran mid = begin + (end - begin) / 2;
        // see which part of the range contains x; keep looking only in that part
        if (x < *mid)
            end = mid;
        else if (*mid < x)
            begin = mid + 1;
            // if we got here, then *mid == x so we're done
        else
            return true;
    }
    return false;
}
```
15. off-the-end values, it always ensure the range is [begin, end). The advantage? (see section 8.2.7)
16. Input and output iterators
    1. input iterator for `copy`
    ```C++
    vector<int> v;
    // read ints from the standard input and append them to v
    copy(istream_iterator<int>(cin), istream_iterator<int>(), back_inserter(v));
    ```
    2. ouput iterator for `copy`
    ```C++
    // write the elements of v each separated from the other by a space
    copy(v.begin(), v.end(), ostream_iterator<int>(cout, " "));
    ```

## Chapter 9

1. Using the `::` before the function name of a non-member function called by a member function.
```c++
doubel Student_info::grad() const
{
    return ::grade(midterm, final, homework);
}
```
2. `const` for member function means this member function will not change the member variable. Only const member functions may be called for const objects.
3. We cannot call non-const functions on const objects. such as `read` memeber on `const Student_info`.
3. When we pass a non-const object to a function that take const reference. The function will treat the object as if it were const, and compiler will only permit it to call const memeber functions.
3. When we pass a nonconst object to a function that takes a const reference, then the function treats that object as if it were const, and the compiler will permit it to call only const members of such objects.
4. difference of `class` and `struct`: default protection. `class` --> private between `{` and first label. 
4. explicitly define a accessor `read` function, `string name() const { return n; }` will return a copy of member variable n instead return a reference, because we don't want the user to modifiy it.
5. The "Synthesized constructor" will initialized the data memebers to a value based on how the object is created. 
   1. if the object is local variable, will be default-initialized (undefined).
   2. If the object is used to init a container element, the members will be value-initialized(zero).
6. Initialization rules:
   1. If an object is of a class type that defines one or more constructors, then the appropriate constructor completely controls initialization of the objects of that class.
   2. If an object is of built-in type, then value-initializing it sets it to zero, and default-initializing it gives it an undefined value.
   3. Otherwise, the object can be only of a class type that does not define any constructors. In that case, value- or default-initializing the object value- or default-initializes each of its data members. This initialization process will be recursive if any of the data members is of a class type with its own constructor.
6. __constructor initializers__ `Student_info::Student_info() : final(0), midterm(0){}`
7. when we create a object:
    1. the implementation allocate memory for the new object.
    2. it initializes the object, as directed by the constructor's initializer list.
    3. it executes the constructor body.
8. The implementation initializes every data member of every object, regardless of whether the constructor initializer list mentions those members. The constructor body may change these initial values subsequently, but the initialization happens before the constructor body begins execution. It is usually better to give a member an initial value explicitly, rather than assigning to it in the body of the constructor. By initializing rather than assigning a value, we avoid doing the same work twice.
9. Constructors with Arguments: `Student_info::Student_info(istream& is) { read(is); }`

## Chapter 10

1. All you can do with a function is to take its address or call it. Any use of function that is not a call is assumed to be taking its address.
2. function pointer declarition: `int (*fp)(int)` in which `fp` is a function pointer, if we have another function definition: `int next(int){ return n+1; }` we can use it like this `fp = &next` or `fp = next`. With & or without it is essentially same.
3. define a function pointer point to a function: `vector<string>(*sp)(const string &) = split;`
3. we can call the next function such as `i = (*fp)(i);` or `i = fp(i);` calling function pointer automatically calling the function itself.
4. function with a return value as a function pointer, can use `typedef`. For example:
```c++
//define analysis_fp as the name of the type of an appropriate pointer
typedef double (*analysis_fp)(const vector<Student_info>&);
//get_analysis_ptr returns a pointer to an analysis function
analysis_fp get_analysis_ptr();
//the alternative and most important trick that has been played in the S2E tcg components.
doubel (*get_analysis_ptr())(const vector<Student_info>&);
```
5. function pointer as parameter to `find_if`, Notice the `Pred` can be any type as long as `f(*begin)` has meaningful value.
```C++
bool is_negative(int n) { return n < 0; }
template<class In, class Pred>
In find_if(In begin, In end, Pred f)
{
    while (begin != end && !f(*begin))
        ++begin;

    return begin;
}
// call it
vector<int>::iterator i = find_if(v.begin(), v.end(), is_negative);
```
5. `<cstddef>` header: `size_t`: unsigned type large enough to hold the size of any object. `ptrdiff_t`: the type of `p - q`, p, q are both pointer.
6. static means only initialize once, not everytime the function calle or the object is initialized.
7. sizeof() operator reports the results in __bytes__.
8. ifstream and ofstream object doesn't like string for file path. It almost always require the name of the file to be a pointer to the initial element of a null-terminated character array.
  1. simplicity. What if the string facilities doesn't exist.
  2. historical. fstream is earlier than string facilities in c++
  3. compatibility. easier to interface with OS file I/O, which typically use such pointers to communicate.
9. using `c_str` member function for string literal. `ifstream infile(filepath.c_str());
10. example that read every file supplied in the commandline.
```c++
int main(int argc, char** argv)
{
    int fail_count = 0;
    for (int i = 1; i < argc; i++){
        ifstream in(argv[i]);
        if(in){
            string s;
            while(getline(in, s))
                cout << s << endl;
        } else {
            cerr << "cannot open file " << argv[i] << endl;
            ++fail_count;
        }
    }
    return fail_count; // very neat trick played here.
}
```
11. Three kinds of memory management
  1. automatica memory management (local variable)
  2. statically allocated memory (`static int x`)
    1. it allocate once and only once before the function contain the statement is ever called.
    1. every call to pointer_to_static will return a pointer to the same object.
    3. the pointer will be valid as long as the program runs, and invalid afterward.
  3. dynamic allocation
12. Allocate object of type T. `new T(args)` i.e. `int* p = new int(32);` allocate a object `int` with initial value is 32.
---

## Chapter 11 (Implement a vector class)

1. template function V.S. template class
```
template <typename T>
T Vec (T a) {
    // function body
}
```
and
```
template <class T> class Vec {
public:
    // interface
private:
    // implementation
}
```
1. What it does when use `new` to allocate memory. (i.e. `new T[n]`)
    1. allocate memory
    2. initialize the element by running the default constructor.
    3. the class T should have a default constructor.
2. A template class type should have the control over how a object created, copied, assigned, or destroyed.
3. `explicit Vec(size_type n) { create(n); }`  mean using the constructor should be explicitly declared, such as `Vec(5)`, not `vec = 5`
4. Type names for the members. Using typedef such as `typedef T* iterator`.
4. Define a overloaded operator: like define a function, the type of the operator(uniary or binary) defines how many parameters the function will have.
5. If the operator is a function that is not a member, then the function has as many arguments as the operator has operands. The first argument is bound to the left operand; the second is bound to the right operand.
6. If the operator is defined as a member function, then its left operand is implicitly bound to the object on which the operator is invoked. Member operator functions, therefore, take one less argument than the operator indicates.
7. Index operator MUST be a member function. `T& operator[](size_type i) { return data[i]; }`, User might also want to only read the element through the index operator, so we can also define another overlaoded version `const T& operator[](size_type i) const { return data[i]; }`. Notice the index operator will return a reference instead of a value.
8. implicitly copying
    1. passing by value in function parameter passing. `vector<int> i; double d; d = median(i);`
    2. return value from a function. (`string line; vector<string> words = split(line);`)
9. explicitly copying
    1. assignment: `vector<Student_info> vec = vs;`
9. __Copy constructor: __ is a member function with the same name as the name of the class
```C++
template<class T> class Vec{
public:
    Vec(const Vec& v); { create(v.begin(), v.end()); } //copy constructor
};
```
    1. using reference because we are defining what it means by copy, so we go deep to the granuality of call by reference to avoid copying.
    2. copying object shouldn't change the original vector, so we use const.
10. Because the copy of vector object is actually copy the pointer, the new copy of the original object contain the same data, point to the same data area. We should make sure they are not contain the same underlying storage when making copies of objects. We should do this: (note the `create` function hasn't been implemented yet)
```C++
template<class T> class Vec{
public:
    Vec(const Vec& v) { create(v.begin(), v.end()); } //copy constructor made a copy.
}
```
11. __assignment operator__: it must be defined as a member function.(may have multiple overloaded versions.) Assignment differs from the copy constructor in that assignment always involves obliterating an existing value (the left-hand side) and replacing it with a new value (the right-hand side).
```C++
template <class T>
class Vec {
public:
    Vec& operator=(const Vec&); //assignment operator
};

template<class T>
Vec<T>& Vec<T>::operator=(const Vec& rhs)
{
    if(&rhs != this){
        uncreate();
        create(rhs.begin(), rhs.end());
    }
    return *this; //why we need '*this' here instead of 'this'
}
```
    1. return reference
    2. uncreate and create
    3. return variable scope
12. How to define a tempalte member function outside of the class?
    * When we should have the `T` in `Vec<T>& Vec<T>::operator=(const Vec& rhs)`?
12. the `oprator=` have two different meanings in C++
    1. Initialization. Such as we do `vector<int> vec = v(10);` or `int a = 10;` we are invoking the copy constructor. Initialization involves creating a new object and giving it a value at the same time. Initialization happens:
        1. In variable declarations (explicitly)
        2. For function parameters on entry to a function (implicitly)
        3. For the return value of a function on return from the function (implicitly)
        4. In constructor initializers (explicitly)
    2. Assignment, we are calling `operator=`. Assignment (operator=) always obliterates a previous value; initialization never does so.
    3. examples:
    ```c++
        string url_ch = "~;/?:@=&$-_.+!*'()," // initialization,(constructor + copy constructor)
        string spaces(url_ch.size(), ' ') ; // initialization
        string y; // initialization
        y = url_ch; // assignment, call the operator= and obliterate a previous value.

        //more complex ones
        vector<string> split(const string&); // function declaration
        vector<string> v; // initialization
        v = split(line); // on entry, initialization of split's parameter from line;
                            // on exit, both initialization of the return value
                            // and assignment to v
    ```
13. The declaration of split above is interesting because it defines a return type that is a class type. Assigning a class type return value from a function is a two-step process: First, the copy constructor is run to copy the return value into a temporary at the call site. Then the assignment operator is run to assign the value of that temporary to the left-hand operand.
13. Constructors always control initialization.
14. The operator= member function always controls assignment.
15. Defalut action regarding the copy constructor, assignment operator, and destructor:
16. rule of three: copy constructor, destructor, and assignment operator. if you defind a class, you probably need the following for copy control and assignment operators.
```C++
T::T() one or more constructors, perhaps with arguments
T::~T() the destructor
T::T(const T&) the copy constructor
T::operator=(const T&) the assignment operator
```
17. the compiler will invoke them whenever an object of our type is created, copied, assigned, or destroyed. Remember that objects may be created, copied, or destroyed implicitly. Whether implicitly or explicitly, the compiler will invoke the appropriate operation.
18. consideration in design a vector class:
    1. constructor
    2. type definition
    3. index and size (overload operators)
    4. copy control
    5. destructor
19. Flexible Memory Management, those functions that used to implement the `create` and `uncreate` functions.
    1. `new` always initialized every object by using constructor `T::T()`. If we want to initialized by ourselves, we have to do it twice.
    2. `allocator<T>` class in `<memory>` library. Members and non member function:
    ```c++
    T* allocate(size_t);
    void deallocate(T*, size_t);
    void construct(T*, const T&) ;
    void destroy(T*);
    template<class Out, class T> void uninitialized_fill(Out, Out, const T&);
    template<class In, class Out> Out uninitialized_copy(In, In, Out);
    ```

## Chapter 12 (Making class objects working like values)

## Chapter 13 (Inheritance)

1. the derived class will not inherit the following: constuctor, assignment operator, and destructor.
2. Keyword `protected` allows the derived class to access the private member of the base.
3. derived class is constructed by the following steps:
    1. allocate memory for the entire object.(base member and derived class member.)
    2. call base constructor to initialize the base part.
    3. initialize the member of the derived class by initializer list.
    4. call constructor of the derived class.
    NOTE: However, it doesn't select which base constructor to run, we have to explicitly involke it.
3. "The derived-class constructor initializer names its base class followed by a (possibly empty) list of arguments. These arguments are the initial values to use in constructing the base- class part; they serve to select the base-class constructor to run in order to initialize the base."
4. If we pass `Grad*` to function that take `Core*`, Compiler convert `grad*` to `Core*` and bind the parameter to a `Core*` type.
4. Static binding V.S. Dynamic binding. "The phrase dynamic binding captures the notion that functions may be bound at run time, as opposed to static bindings that happen at compile time."
5. Virtual Function: (mainly for pointer and references, not for explicit object, because the later is bind to the function in compile time.)
    1. It come into being in the following accasion:
    ```c++
    bool compare_grade(const Core& c1, const Core& c2)
    {
        return c1.grade() < c2.grade();
    }
    ```
    which function to call, it has to be decide in run time. The reason is that the parameter type `const Core&` can also accept a type `Grade*`.
    2. More examples.
    ```c++
    Core c;
    Grad g;
    Core *p;
    Core& r = g;
    c.grade();	// statically bound to Core::grade()
    g.grade();	// statically bound to Core::grade()
    p->grade(); // dynamically bound, depending on the type of the object to which p points
    r.grade();	// dynamically bound, depending on the type of the object to which p points
    ```
6. if we defind the `bool compare_grade(const Core C1, const Core C2)`, if we pass `Grad` to it, it __cut down__ to its core part. The two `grade()` would be identically from `Core`. If we define pointer parameters, the compiler will convert `Grad*` to a `Core*`, and would bind the pointer to the `Core` part of the `Grad` object.
7. polymorphism: one type (base type) stand for many types (by reference and poitners). "C++ supports polymorphism through the dynamic-binding properties of virtual functions. When we call a virtual through a pointer or reference, we make a polymorphic call. The type of the reference (or pointer) is fixed, but the type of the object to which it refers (or points) can be the type of the reference (or pointer) or any type derived from it. Thus, we can potentially call one of many functions through a single type."
7. virtual function must be defined, regardless of whether the program calles them.
8. virtual destructor: usually in base not in derived class. it usually empty if not other special thing need todo.
9. virtual properties are inherented, such as virtual function or virtual destructor, the keyword "virtual" only need to be defined in the base class, and no need to redeclared in derived class.
10. virtual destructor: when you delete the heap memory using the command delete, the pointer operand for delete might be more than one class types. you have give the compiler right indication what object space to release, we use the virutal destructor to do this, for example:
```C++
class Core(){
public:
        virutal ~Core(){}//empty destructor is enough
}
```
In this case the delete will automatically select the synthesized approperiate destructor for base class.
11. A virtual destructor is needed any time it is possible that an object of derived type is destroyed through a pointer to base.
12. A virtual destructor is inherited and we don't need to add the virtual destructor to the derived class such as `Grad`. 
13. Programming technique: handle class. hide the pointer manipulations and encapsulate the pointer to `Core`.
10. static member function. Static member functions differ from ordinary member functions in that they do not operate on an object of the class type. Unlike other member functions, they are associated with the class, not with a particular object.
11. How to implement copy constructor? give the handle class a virtual function `clone()` to implement the copy constructor. another wrapper!!!
```C++
class Core {
    friend class Student_info;
protected:
    virtual Core* clone() const { return new Core(*this);}
    //as before.
};
```
Notice that the copy constructor didn't defined explicitly. It is synthesized by the implementation. (default copy constructor)
15. Ordinarily, when a derived class redefines a function from the base class, it does so exactly: the parameter list and the return type are identical.  However, if the base-class function returns a pointer (or reference) to a base class, then the derived-class function can return a pointer (or reference) to a corresponding derived class.
16. "Finally, the objects that were allocated inside the read for the
    Student_info function will be automatically freed when we exit main. On
    exiting main, the vector will be destroyed. The destructor for vector will
    destroy each element in students, which will cause the destructor for
    Student_info to be run. When that destructor runs, it will delete each of
    the objects allocated in read."
12. look at the following piece of code: what will happen, if you mistake on the type of the class.
```C++
vector<Core> students;
Grad g(cin);            // read a Grad
students.push_back(g);  // Store only the core part of the object.
```
What will happen is that push_back will expect that it was given a Core object,
and will construct a Core element, copying only the Core parts of the object,
ignoring whatever is specific to the Grad class.

13. We can control which function to call by specify the scope operator, such as when r is a reference to Grad, we can call the regrade function of Core. 
```
r.Core::regrade(100);
```
14. keep in mind that base function is always hiden if you call the derived class function member when the two are have same form (see 13.6.2 in page 347)
