//#include "../../include/expressions/AbsExpression.h"
//
//#include "../../include/Variable.h"
//
//AbsExpression::AbsExpression(Expression &a) :
//        Expression({a}, false),
//        mExpr1(a) {}
//
//int AbsExpression::evaluate(SearchNode &context, int time) const
//{
//    int val = mExpr1.evaluate(context, time);
//    return val < 0 ? (-1 * val) : val;
//}
