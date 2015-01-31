package chat

import (
	"fmt"
)

const (
	NAME_PREFIX = "User "
)

func getUniqName() string {
	return fmt.Sprintf("%s%d", NAME_PREFIX, "test" )
}
