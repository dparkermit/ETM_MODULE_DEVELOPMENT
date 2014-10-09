void ETMCanSetValue(ETMCanMessage* message_ptr) {
  unsigned int index_word;
  index_word = message_ptr->word_3;
}

#ifdef __MASTER_MODULE
void ETMCanUpdateStatus(ETMCanMessage* message_ptr) {
  
}
#endif

#ifndef __MASTER_MODULE
void ETMCanExecuteCMD(ETMCanMessage* message_ptr) {
  
}
#endif


#ifndef __MASTER_MODULE
void ETMCanReturnValue(ETMCanMessage* message_ptr) {

}
#endif
