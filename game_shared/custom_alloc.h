/************************************
	simple memory allocator
	by BUzer
************************************/

template <class T>
class MemBlock
{
	typedef struct chunk_s {
		int		next;
		T		data;
	} chunk_t;

public:
	MemBlock(int numElements)
	{
		// элемент 0 используетс€ в качестве начала списка зан€тых €чеек
		m_iArraySize = numElements + 1;
		m_pArray = new chunk_t[m_iArraySize];		

		if (!m_pArray)
		{
			m_iArraySize = 1;
			m_iFirstFree = m_iArraySize;
			return;
		}

		Clear();
	}

	~MemBlock()
	{
		delete[] m_pArray;
		m_pArray = NULL;
	}

	void Clear( void )
	{
		if (m_iArraySize > 1)
		{
			m_pArray[0].next = 0; // если он ссылаетс€ сам на себ€, значит список зан€тых пуст
			m_iFirstFree = 1;

			for (int i = 1; i < m_iArraySize; ++i)
				m_pArray[i].next = i + 1;
		}
	}

	T* Allocate( void )
	{
		if (m_iFirstFree != m_iArraySize)
		{
			int savedFirstFree = m_pArray[m_iFirstFree].next;
			m_pArray[m_iFirstFree].next = m_pArray[0].next; // добавл€ем свободную €чейку в
			m_pArray[0].next = m_iFirstFree;				//   список зан€тых
			m_iFirstFree = savedFirstFree;	// исключаем €чейку из списка свободных
			return &(m_pArray[m_pArray[0].next].data);
		}
		else
			return NULL;
	}

	bool IsClear( void )
	{
		return m_pArray[0].next ? false : true;
	}

	bool StartPass( void )
	{
		m_iCurrent = 0; // начинаем обход с нулевого элемента

		if (m_iArraySize > 1)
			return true;
		else
			return false;
	}

	T* GetCurrent( void )
	{
		int retindex = m_pArray[m_iCurrent].next;
		if (!retindex)
			return NULL;

		return &(m_pArray[retindex].data);
	}

	void MoveNext( void )
	{
		m_iCurrent = m_pArray[m_iCurrent].next;
	}

	void DeleteCurrent( void )
	{
		int delindex = m_pArray[m_iCurrent].next;
		m_pArray[m_iCurrent].next = m_pArray[delindex].next; // выбрасываем элемент из цепи зан€тых
		m_pArray[delindex].next = m_iFirstFree;
		m_iFirstFree = delindex; // включаем элемент в начало цепи свободных
	}

private:
	chunk_t	*m_pArray;
	int	m_iArraySize;
	int	m_iCurrent;	// дл€ прохождени€ через массив
	
	int	m_iFirstFree;	// начало списка свободных элементов
};


