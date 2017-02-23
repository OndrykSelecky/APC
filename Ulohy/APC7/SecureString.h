#pragma once
#include "Protect.h"
#include "SecureStringUnprotected.h"

/*
Pozn. Som dosù zm‰ten˝ z move a tohoto zadania, preto d˙fam, ûe som to dobre pochopil :D.
Pouziva char* , nie std::string. Preto som pouzil malloc a realloc pri ˙prav·ch. 
Na koniec stringu je vzdy pridan˝ '\0'. Pr·zdny string je dlzky 1 a obsahuje len prazdny znak.
*/


class SecureString
{
public:

	SecureString();
		
	~SecureString();
		
	SecureString(const char *str);
	SecureString(SecureString& other);
	SecureString(SecureString&& other);
	SecureString &operator=(SecureString& rhs);
	SecureString &operator=(SecureString&& rhs);

	SecureString &operator+=(SecureString& rhs);

	size_t Length();
	void Clear();
	void SetAt(size_t position, char new_char);
	void AppendChar(char new_char);
	void RemoveAt(size_t position);

	SecureStringUnprotected GetUnprotected() const;

	void print();

private:
	char *m_string = nullptr;
	size_t m_length;
	size_t m_buffer_length;

	size_t count_buffer_length(size_t buffer_length);
	void init_empty();
};
