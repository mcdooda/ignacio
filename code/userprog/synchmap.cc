#ifdef CHANGED

// ne pas ajouter dans le Makefile!


template <class T, class U> SynchMap<T, U>::SynchMap() :
	std::map<T, U>(),
	sem("SynchMap read", 1)
{
	
}

template <class T, class U> bool SynchMap<T, U>::TryGet(T key, U& value)
{
	typename SynchMap<T, U>::iterator it = this->find(key);
	if (it != this->end())
	{
		value = it->second;
		return true;
	}
	else
		return false;
}

template <class T, class U> void SynchMap<T, U>::Erase(T key)
{
	typename SynchMap<T, U>::iterator it = this->find(key);
	if (it != this->end())
		this->erase(it);
}

template <class T, class U> void SynchMap<T, U>::SynchAdd(T key, U value)
{
	P();
	(*this)[key] = value;
	V();
}

template <class T, class U> const U& SynchMap<T, U>::SynchGet(T key)
{
	P();
	const U& value = (*this)[key];
	V();
	return value;
}

template <class T, class U> bool SynchMap<T, U>::SynchTryGet(T key, U& value)
{
	P();
	bool found = TryGet(key, value);
	V();
	return found;
}

template <class T, class U> void SynchMap<T, U>::SynchErase(T key)
{
	P();
	Erase(key);
	V();
}

template <class T, class U> void SynchMap<T, U>::P()
{
	sem.P();
}

template <class T, class U> void SynchMap<T, U>::V()
{
	sem.V();
}


#endif


