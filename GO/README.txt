
go run main.go
cd hashtable
go test -v -coverprofile=cover.out
go tool cover -html=cover.out -o cover.html

xdg-open cover.html

