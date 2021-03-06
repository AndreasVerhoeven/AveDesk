//package kgs.assignmentB_CaseBase;

import java.io.Serializable;
import java.util.Vector;
import java.util.Iterator;

public class Cluster extends Interpretation implements Serializable 
{
	public Cluster(Interpretation interpretation)
	{
		super(interpretation.m_label);
		m_cases = interpretation.m_cases;
	}

	public Vector getIndex()
	{
		Vector cases = getCases();
		Vector index = new Vector();

		if (null == cases)
			return null;
		
		Iterator iter = cases.iterator();
		while (iter.hasNext())
		{
			Case curCase = (Case) iter.next();
			if(curCase.getIsInIndex())
				index.add(curCase);
		}

		return index;

	}
}
