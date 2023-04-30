; ModuleID = 'advcalc2ir'
declare i32 @printf(i8*, ...)

@print.str = constant [4 x i8] c"%d\0A\00"

define i32 @main() {
	%x = alloca i32
	%y = alloca i32
	%zvalue = alloca i32
	%k = alloca i32
	store i32 3, i32* %x
	store i32 5, i32* %y
	%1 = load i32, i32* %y
	%2 = add i32 1, %1
	%3 = load i32, i32* %x
	%4 = mul i32 %3, %2
	%5 = add i32 23, %4
	store i32 %5, i32* %zvalue
	call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %5)
	%7 = load i32, i32* %y
	%8 = load i32, i32* %x
	%9 = sub i32 %8, %7
	%10 = load i32, i32* %zvalue
	%11 = sub i32 %10, %10
	store i32 %11, i32* %k
	%12 = load i32, i32* %y
	%13 = mul i32 3, %12
	%14 = add i32 2, 5
	%15 = mul i32 1, %14
	%16 = mul i32 %15, %15
	%17 = load i32, i32* %x
	%18 = add i32 %17, %16
	store i32 %18, i32* %k
	%19 = load i32, i32* %k
	%20 = add i32 %19, 1
	call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %20)
	ret i32 0
}