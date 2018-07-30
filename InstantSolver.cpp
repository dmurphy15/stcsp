#include "InstantSolver.h"

#include "Constraint.h"
#include "Variable.h"

bool operator==(InstantSolver &lhs, InstantSolver &rhs) {
    return (lhs.mInputAssignments == rhs.mInputAssignments) && (lhs.mConstraints == rhs.mConstraints);
}