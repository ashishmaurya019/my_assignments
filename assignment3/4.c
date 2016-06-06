/* kill -SIGSTOP <kill>
 * when we stop a pdf viewer by giving a SIGSTOP signal using kill command
 * it's stopped; state is changed to T-stopped from S-sleeping
 * In top command we see a STOPPED 1
 * And we are not able to access it meaning it not scrolling down and up....
 *
 * kill -SIGCONT <pid>
 * On giving SIGCONT signal it is resumed to its previous state
 * state is changed to S and in top command 0 STOPPED
 * And most important...we are able to access it...read write...!!!  
