runNaive:
	dotnet run --project DiningPhilosophers.Cli -- \
  --mode step --strategy naive --steps 1000000 --render-every 1000 --names names.txt
runButler:
	dotnet run --project DiningPhilosophers.Cli -- \
  --mode step --strategy coordinated --steps 1000000 --render-every 1000

runReal:
	dotnet run --project DiningPhilosophers.Cli -- \
  --mode rt --strategy naive --duration 20000 --render-period 2000

runHost:
	dotnet run --project DiningPhilosophers.App.GenericHost 

help:
	@echo "runNaive"
	@echo "runButler"
	@echo "runReal"
	@echo "runHost"
