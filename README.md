# PHLang - A Simple Educational Programming Language

[![CC BY 4.0](https://licensebuttons.net/l/by/4.0/88x31.png)](https://creativecommons.org/licenses/by/4.0/)

**PHLang** (PH Language) is a tiny educational programming language implemented in C.  
It is designed to help beginners learn programming concepts like variables, loops, arithmetic, and printing output in a simple and clear way.

---

## Features

- Print statements: `print "Hello World";`
- Basic arithmetic expressions: `1 + 2 * 3`
- Variable assignment: `x = 5;`
- Simple `while` loops: `while (x > 0) { ... }`
- Blocks using `{ }`
- Easy-to-read syntax for educational purposes

---

## Syntax Rules

- Statements end with `;`
- Strings must be in double quotes: `"Hello World"`
- Numbers are integers only
- Operators: `+`, `-`, `*`, `/`, `(`, `)`
- Keywords: `print`, `while`

---

## Example Programs

### Hello World

```ph
print "Hello World";
```
### Countdown
```
x = 5;
while (x > 0) {
    print "Countdown: ";
    print x;
    x = x - 1;
}
print "Blast off!";
```
### Arithmetic Example
```
a = 10;
b = 20;
c = a + b * 2;
print c;
```
### Building & Running

#### Compile the engine:
```
gcc phlang_engine/ph_engine.c -o phlang


Run a PH program:

./phlang ph_programs/hello.ph

```

### Make sure your PH programs are saved with the .ph extension.

### Contributing
```
Contributions, suggestions, and bug reports are welcome!
Feel free to fork the repo, make improvements, and submit a pull request.
```
### License
```
This project is licensed under the Creative Commons Attribution 4.0 International License (CC BY 4.0)
.

Attribution Requirements:

Credit the creator: CHRISTINAPAUL

Provide a link to the license: CC BY 4.0

Indicate if changes were made

Do not imply endorsement by the creator

You are free to share, copy, modify, and distribute, even commercially, as long as the above conditions are met.

Creator Info:

Name: CHRISTIANPAUL

GitHub Repo: PH_LANGAUGE

Facebook: CHRISTIANPAUL

Contact

For questions or support, contact: attendance1978@gmail.com
 (optional)

PHLang is an educational project to teach programming basics in a simple and approachable way. Enjoy learning and building with it!


---

If you want, I can also **create a complete `LICENSE` file** for your repo that includes the full **CC BY 4.0 legal text**, ready to paste into your GitHub repo.  

Do you want me to generate that too?
