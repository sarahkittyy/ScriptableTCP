# ScriptableTCP

An easy way to set up & connect to a custom TCP socket listener.

## Building

Dependencies:
* Lua https://www.lua.org/
* SFML https://github.com/SFML/SFML

```bash
git clone https://github.com/sarahkittyy/ScriptableTCP.git
cd ScriptableTCP

mkdir build
cd build
cmake ..
make
```

## Usage

```bash
./build/tcp.out
```

## Scripting

Scripts are written in lua.

* All standard libraries are gone.
* I/O:
    * No input. The string of data passed is put into the lua script as the global list `arg`.
    * I.E. Receiving `"Hello World"` yields `arg = ["Hello", "World"]`
    * `print()` is flushed and sent back only after the script has finished executing.
* Extraneous functions:
    * `tonumber()` -> Converts a string to a number.
    * `tostring()` -> Converts a number to a string.
    
## Script Example

```lua
-- Server-side script.
function factorial(x)
    if x <= 1 then
        return 1
    else
        return x * factorial(x-1)
    end
end

input = tonumber(arg[1])
print(factorial(input))
```
```bash
# Bash netcat connection.
echo "5" | netcat [ip] [port]
# Output: 120
```

## TODO

* Finish up client-side GUI.