run-clean: clean restore build run

all: clean restore build

clean:
	dotnet clean -o ../../build

restore:
	dotnet restore

build:
	dotnet build -o ../../build

run:
	dotnet run
