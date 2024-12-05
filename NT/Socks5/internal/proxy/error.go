
package proxy

import "fmt"

type ErrAuthRequestParsing struct {
	message string
}

func NewErrAuthRequestParsing(message string) *ErrAuthRequestParsing {
	return &ErrAuthRequestParsing{message: message}
}

func (e *ErrAuthRequestParsing) Error() string {
	return fmt.Sprintf("Authentication request parsing error: %v", e.message)
}

type ErrAuthReplySending struct {
	message string
}

func NewErrAuthReplySending(message string) *ErrAuthReplySending {
	return &ErrAuthReplySending{message: message}
}

func (e *ErrAuthReplySending) Error() string {
	return fmt.Sprintf("Authentication reply sending error: %v", e.message)
}

type ErrCommandRequestParsing struct {
	message string
}

func NewErrCommandRequestParsing(message string) *ErrCommandRequestParsing {
	return &ErrCommandRequestParsing{message: message}
}

func (e *ErrCommandRequestParsing) Error() string {
	return fmt.Sprintf("Command request parsing error: %v", e.message)
}

type ErrCommandReplySending struct {
	message string
}

func NewErrCommandReplySending(message string) *ErrCommandReplySending {
	return &ErrCommandReplySending{message: message}
}

func (e *ErrCommandReplySending) Error() string {
	return fmt.Sprintf("Command reply sending error: %v", e.message)
}

type ErrPeerConnectionCreating struct {
	message string
}

func NewErrPeerConnectionCreating(message string) *ErrPeerConnectionCreating {
	return &ErrPeerConnectionCreating{message: message}
}

func (e *ErrPeerConnectionCreating) Error() string {
	return fmt.Sprintf("Peer connection creating error: %v", e.message)
}

type ErrDNSResolving struct {
	message string
}

func NewErrDNSResolving(message string) *ErrDNSResolving {
	return &ErrDNSResolving{message: message}
}

func (e *ErrDNSResolving) Error() string {
	return fmt.Sprintf("DNS resolving error: %v", e.message)
}
