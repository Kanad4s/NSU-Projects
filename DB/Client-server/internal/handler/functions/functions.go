package functions

import "time"

func FormatDate(t time.Time) string {
	return t.Format("2006-01-02")
}

func FormatDateWithNull(t *time.Time) string {
	if t == nil {
		return "—"
	}
	return t.Format("2006-01-02")
}
