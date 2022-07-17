@echo off

for /l %%i in (1, 1, 100) do (
    echo %%i

    gen.exe > input.txt
    main.exe < input.txt > output.txt
    main2.exe < input.txt > answer.txt

    fc output.txt answer.txt || goto :out
)

:out