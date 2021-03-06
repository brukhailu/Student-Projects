#include "ast_prod.h"

ast_t *new_function(char *args, ast_t *body) {
  ast_t *fun_r = malloc(sizeof *fun_r);
  fun_r->fun.type = TYPE_FUN;
  fun_r->fun.args = args;
  fun_r->fun.body = body;
  return fun_r;
}

ast_t *new_let(char *name, ast_t *value, ast_t *body) {
  ast_t *let_r = malloc(sizeof *let_r);
  let_r->let.type = TYPE_LET;
  let_r->let.name = name;
  let_r->let.value = value;
  let_r->let.body = body;
  return let_r;
}

ast_t *new_let_rec(char *name, ast_t *value, ast_t *body) {
  ast_t *let_r = new_let(name, value, body);
  let_r->type = TYPE_LET_R;
  return let_r;
}

ast_t *new_nil() {
  ast_t *nil_r = malloc(sizeof *nil_r);
  nil_r->type = TYPE_NIL;
  return nil_r;
}

ast_t *new_true() {
  ast_t *true_r = malloc(sizeof *true_r);
  true_r->boolean.type = TYPE_BOOL;
  true_r->boolean.value = true;
  return true_r;
}

ast_t *new_false() {
  ast_t *false_r = malloc(sizeof *false_r);
  false_r->boolean.type = TYPE_BOOL;
  false_r->boolean.value = false;
  return false_r;
}

ast_t *new_var(char *name) {
  ast_t *var_r = malloc(sizeof *var_r);
  var_r->var.type = TYPE_VAR;
  var_r->var.name = name;
  return var_r;
}

ast_t *new_num(int value) {
  ast_t *num_r = malloc(sizeof *num_r);
  num_r->num.type = TYPE_NUM;
  num_r->num.value = value;
  return num_r;
}

ast_t *new_apply(ast_t *fun, ast_t *arg) {
  ast_t *apply_r = malloc(sizeof *apply_r);
  apply_r->apply.type = TYPE_APPLY;
  apply_r->apply.fun = fun;
  apply_r->apply.arg = arg;
  return apply_r;
}

ast_t *new_cons(ast_t *head, ast_t *tail) {
  ast_t *cons_r = malloc(sizeof *cons_r);
  cons_r->cons.type = TYPE_CONS;
  cons_r->cons.head = head;
  cons_r->cons.tail = tail;
  return cons_r;
}

ast_t *new_match(ast_t *expr, ast_t *list) {
  ast_t *match_r = malloc(sizeof *match_r);
  match_r->match.type = TYPE_MATCH;
  match_r->match.expr = expr;
  match_r->match.list = list;
  return match_r;
}

ast_t *new_bin(type_t type, ast_t *left, ast_t *right) {
  ast_t *bin_r = malloc(sizeof *bin_r);
  bin_r->bin_op.type = type;
  bin_r->bin_op.left = left;
  bin_r->bin_op.right = right;
  return bin_r;
}

ast_t *new_cond(ast_t *cond, ast_t *then, ast_t *elsse) {
  ast_t *cond_r = malloc(sizeof *cond_r);
  cond_r->cond.type = TYPE_COND;
  cond_r->cond.cond = cond;
  cond_r->cond.then = then;
  cond_r->cond.elsse = elsse;
  return cond_r;
}

ast_t *new_nil_l(ast_t *body) {
  ast_t *nil_r = malloc(sizeof *nil_r);
  nil_r->nil_l.type = TYPE_NIL_L;
  nil_r->nil_l.body = body;
  return nil_r;
}

ast_t *new_cons_l(ast_t *cons, ast_t *body) {
  ast_t *cons_r = malloc(sizeof *cons_r);
  cons_r->cons_l.type = TYPE_CONS_L;
  cons_r->cons_l.cons = cons;
  cons_r->cons_l.body = body;
  return cons_r;
}

ast_t *new_full_l(ast_t *first, ast_t *second) {
  ast_t *full_r = malloc(sizeof *full_r);
  full_r->full_l.type = TYPE_FULL_L;
  full_r->full_l.first = first;
  full_r->full_l.second = second;
  return full_r;
}

void free_program(ast_t *program) {
  switch (program->type) {
    case TYPE_NIL:
    free(program);
    break;
    case TYPE_BOOL:
    free(program);
    break;
    case TYPE_NUM:
    free(program);
    break;
    case TYPE_VAR:
    free(program->var.name);
    free(program);
    break;
    case TYPE_FUN:
    free_program(program->fun.body);
    free(program->fun.args);
    free(program);
    break;
    case TYPE_APPLY:
    free_program(program->apply.fun);
    free_program(program->apply.arg);
    free(program);
    break;
    case TYPE_COND:
    free_program(program->cond.cond);
    free_program(program->cond.then);
    free_program(program->cond.elsse);
    free_program(program);
    break;
    case TYPE_MATCH:
    free_program(program->match.expr);
    free_program(program->match.list);
    free(program);
    break;
    case TYPE_CONS:
    free_program(program->cons.head);
    free_program(program->cons.tail);
    free(program);
    break;
    case TYPE_ADD:
    free_program(program->bin_op.left);
    free_program(program->bin_op.right);
    free(program);
    break;
    case TYPE_MUL:
    free_program(program->bin_op.left);
    free_program(program->bin_op.right);
    free(program);
    break;
    case TYPE_SUB:
    free_program(program->bin_op.left);
    free_program(program->bin_op.right);
    free(program);
    break;
    case TYPE_LEQ:
    free_program(program->bin_op.left);
    free_program(program->bin_op.right);
    free(program);
    break;
    case TYPE_GEQ:
    free_program(program->bin_op.left);
    free_program(program->bin_op.right);
    free(program);
    break;
    case TYPE_LET:
    free_program(program->let.value);
    free_program(program->let.body);
    free(program->let.name);
    free(program);
    break;
    case TYPE_LET_R:
    free_program(program->let.value);
    free_program(program->let.body);
    free(program->let.name);
    free(program);
    break;
    case TYPE_NIL_L:
    free_program(program->nil_l.body);
    free(program);
    break;
    case TYPE_CONS_L:
    free_program(program->cons_l.cons);
    free_program(program->cons_l.body);
    free(program);
    break;
    case TYPE_FULL_L:
    free_program(program->full_l.first);
    free_program(program->full_l.second);
    free(program);
    break;
  }
}
